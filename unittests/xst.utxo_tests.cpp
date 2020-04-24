/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/testing/tester.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>

#include <boost/test/unit_test.hpp>

#include <contracts.hpp>

#include "fork_test_utilities.hpp"

#include <eosio/testing/tester.hpp>
#include <eosio/chain/exceptions.hpp>
#include <eosio/chain/account_object.hpp>
#include <eosio/chain/contract_table_objects.hpp>
#include <eosio/chain/block_summary_object.hpp>
#include <eosio/chain/global_property_object.hpp>
#include <eosio/chain/generated_transaction_object.hpp>
#include <eosio/chain/wasm_interface.hpp>
#include <eosio/chain/resource_limits.hpp>

#include <fc/crypto/digest.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/exception/exception.hpp>
#include <fc/variant_object.hpp>

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;
using namespace fc::ecc;
using namespace std;
using namespace fc::crypto;

using mvo = fc::mutable_variant_object;

struct input {

    uint64_t id;
    signature_type sig;

    friend bool operator == ( const input& lhs, const input& rhs ) {
        return tie( lhs.id, lhs.sig ) == tie( rhs.id, rhs.sig );
    }
    friend bool operator != ( const input& lhs, const input& rhs ) {
        return tie( lhs.id, lhs.sig ) == tie( rhs.id, rhs.sig );
    }
};

struct output {
    public_key_type pk;
    account_name account;
    asset quantity;

    friend bool operator == ( const output& lhs, const output& rhs ) {
        return tie( lhs.pk, lhs.account, lhs.quantity ) == tie( rhs.pk, rhs.account, rhs.quantity );
    }
    friend bool operator != ( const output& lhs, const output& rhs ) {
        return tie( lhs.pk, lhs.account, lhs.quantity ) == tie( rhs.pk, rhs.account, rhs.quantity );
    }
};

FC_REFLECT( input, (id)(sig) )
FC_REFLECT( output, (pk)(account)(quantity) )

class xst_utxo_tester : public tester {
public:

    xst_utxo_tester() {
        produce_blocks( 2 );

        create_accounts( { N(alice), N(bob), N(carol), N(xst.utxo) } );
        produce_blocks( 2 );

        set_code( N(xst.utxo), contracts::xst_utxo_wasm() );
        set_abi( N(xst.utxo), contracts::xst_utxo_abi().data() );

        produce_blocks();

        const auto& accnt = control->db().get<account_object,by_name>( N(xst.utxo) );
        abi_def abi;
        BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
        abi_ser.set_abi(abi, abi_serializer_max_time);
    }



    action_result push_action( const account_name& signer, const action_name &name, const variant_object &data ) {
        string action_type_name = abi_ser.get_action_type(name);

        action act;
        act.account = N(xst.utxo);
        act.name    = name;
        act.data    = abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

        return base_tester::push_action( std::move(act), signer.to_uint64_t());
    }

    fc::variant get_stats( const string& symbolname )
    {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(xst.utxo), name(symbol_code), N(stat), name(symbol_code) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "currency_stats", data, abi_serializer_max_time );
    }

    fc::variant get_account( account_name acc, const string& symbolname)
    {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(xst.utxo), acc, N(accounts), name(symbol_code) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "account", data, abi_serializer_max_time );
    }

    fc::variant get_utxo( account_name acc, const string& symbolname)
    {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(xst.utxo), acc, N(utxos), name(symbol_code) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "utxo", data, abi_serializer_max_time );
    }

    action_result create( account_name issuer,
                          asset        maximum_supply ) {

        return push_action( N(xst.utxo), N(create), mvo()
                ( "issuer", issuer)
                ( "maximum_supply", maximum_supply)
        );
    }

    action_result update( account_name issuer,
                          asset        maximum_supply ) {

        return push_action( N(xst.utxo), N(update), mvo()
                ( "issuer", issuer)
                ( "maximum_supply", maximum_supply)
        );
    }

    action_result issue( account_name issuer, account_name to, asset quantity, string memo ) {
        return push_action( issuer, N(issue), mvo()
                ( "to", to)
                ( "quantity", quantity)
                ( "memo", memo)
        );
    }

    action_result transfer( account_name from,
                            account_name to,
                            asset        quantity,
                            string       memo ) {
        return push_action( from, N(transfer), mvo()
                ( "from", from)
                ( "to", to)
                ( "quantity", quantity)
                ( "memo", memo)
        );
    }

    action_result loadutxo( account_name from,
                            public_key_type   pk,
                            asset        quantity) {
        return push_action( from, N(loadutxo), mvo()
                ( "from", from)
                ( "pk", pk)
                ( "quantity", quantity)
        );
    }

    action_result transferutxo( const account_name& payer,
                                const std::vector<input>&   inputs,
                                const std::vector<output>&  outputs,
                                const string& memo)  {
        return push_action( payer, N(transferutxo), mvo()
                ( "payer", payer)
                ( "inputs", inputs)
                ( "outputs", outputs)
                ( "memo", memo)

        );
    }


    abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(xst_utxo_tests)

BOOST_FIXTURE_TEST_CASE( create_tests, xst_utxo_tester ) try {

    // auto token = create( N(alice), asset::from_string("1000.0000 UTXO"));
    // auto stats = get_stats("4,UTXO");
    // REQUIRE_MATCHING_OBJECT( stats, mvo()
    //         ("supply", "0.0000 UTXO")
    //         ("max_supply", "1000.0000 UTXO")
    //         ("issuer", "alice")
    // );
    // produce_blocks(1);

    auto token = create( N(alice), asset::from_string("1000.000 TKN"));
   auto stats = get_stats("3,TKN");
   REQUIRE_MATCHING_OBJECT( stats, mvo()
      ("supply", "0.000 TKN")
      ("max_supply", "1000.000 TKN")
      ("issuer", "alice")
   );
   produce_blocks(1);

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( create_negative_max_supply, xst_utxo_tester ) try {

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "max-supply must be positive" ),
             create( N(alice), asset::from_string("-1000.000 UTXO"))
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( symbol_already_exists, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("100 UTXO"));
    auto stats = get_stats("0,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0 UTXO")
            ("max_supply", "100 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "token with symbol already exists" ),
                         create( N(alice), asset::from_string("100 UTXO"))
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( create_max_supply, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("4611686018427387903 UTXO"));
    auto stats = get_stats("0,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0 UTXO")
            ("max_supply", "4611686018427387903 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    asset max(10, symbol(SY(0, TSX)));
    share_type amount = 4611686018427387904;
    static_assert(sizeof(share_type) <= sizeof(asset), "asset changed so test is no longer valid");
    static_assert(std::is_trivially_copyable<asset>::value, "asset is not trivially copyable");
    memcpy(&max, &amount, sizeof(share_type)); // hack in an invalid amount

    BOOST_CHECK_EXCEPTION( create( N(alice), max) , asset_type_exception, [](const asset_type_exception& e) {
        return expect_assert_message(e, "magnitude of asset amount must be less than 2^62");
    });


} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( create_max_decimals, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1.000000000000000000 UTXO"));
    auto stats = get_stats("18,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0.000000000000000000 UTXO")
            ("max_supply", "1.000000000000000000 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    asset max(10, symbol(SY(0, TSX)));
    //1.0000000000000000000 => 0x8ac7230489e80000L
    share_type amount = 0x8ac7230489e80000L;
    static_assert(sizeof(share_type) <= sizeof(asset), "asset changed so test is no longer valid");
    static_assert(std::is_trivially_copyable<asset>::value, "asset is not trivially copyable");
    memcpy(&max, &amount, sizeof(share_type)); // hack in an invalid amount

    BOOST_CHECK_EXCEPTION( create( N(alice), max) , asset_type_exception, [](const asset_type_exception& e) {
        return expect_assert_message(e, "magnitude of asset amount must be less than 2^62");
    });

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( update_tests, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1000.0000 UTXO"));
    produce_blocks(1);
    update( N(alice), asset::from_string("2000.0000 UTXO"));
    auto stats = get_stats("4,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0.0000 UTXO")
            ("max_supply", "2000.0000 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    } FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( mismatch_symbol, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1000.0000 UTXO"));
    produce_blocks(1);

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "token with symbol doesn't exists" ),
            update( N(alice), asset::from_string("1000.0000 TSX"))
    );
    produce_blocks(1);

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( update_negative_max_supply, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1000.0000 UTXO"));
    produce_blocks(1);
    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "max-supply must be positive" ),
                         update( N(alice), asset::from_string("-1000.000 UTXO"))
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( update_max_supply, xst_utxo_tester ) try {

    create( N(alice), asset::from_string("1000 UTXO"));
    produce_blocks(1);
    auto token = update( N(alice), asset::from_string("4611686018427387903 UTXO"));
    auto stats = get_stats("0,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0 UTXO")
            ("max_supply", "4611686018427387903 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    asset max(10, symbol(SY(0, TSX)));
    share_type amount = 4611686018427387904;
    static_assert(sizeof(share_type) <= sizeof(asset), "asset changed so test is no longer valid");
    static_assert(std::is_trivially_copyable<asset>::value, "asset is not trivially copyable");
    memcpy(&max, &amount, sizeof(share_type)); // hack in an invalid amount

    BOOST_CHECK_EXCEPTION( update( N(alice), max) , asset_type_exception, [](const asset_type_exception& e) {
        return expect_assert_message(e, "magnitude of asset amount must be less than 2^62");
    });


} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( update_max_decimals, xst_utxo_tester ) try {

    create( N(alice), asset::from_string("1.000000000000000000 UTXO"));
    produce_blocks(1);
    auto token = update( N(alice), asset::from_string("2.000000000000000000 UTXO"));
    auto stats = get_stats("18,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "0.000000000000000000 UTXO")
            ("max_supply", "2.000000000000000000 UTXO")
            ("issuer", "alice")
    );
    produce_blocks(1);

    asset max(10, symbol(SY(0, TSX)));
    //1.0000000000000000000 => 0x8ac7230489e80000L
    share_type amount = 0x8ac7230489e80000L;
    static_assert(sizeof(share_type) <= sizeof(asset), "asset changed so test is no longer valid");
    static_assert(std::is_trivially_copyable<asset>::value, "asset is not trivially copyable");
    memcpy(&max, &amount, sizeof(share_type)); // hack in an invalid amount

    BOOST_CHECK_EXCEPTION( update( N(alice), max) , asset_type_exception, [](const asset_type_exception& e) {
        return expect_assert_message(e, "magnitude of asset amount must be less than 2^62");
    });

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( issue_tests, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1000.000 UTXO"));
    produce_blocks(1);

    issue( N(alice), N(alice), asset::from_string("500.000 UTXO"), "hola" );

    auto stats = get_stats("3,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "500.000 UTXO")
            ("max_supply", "1000.000 UTXO")
            ("issuer", "alice")
    );

    auto alice_balance = get_account(N(alice), "3,UTXO");
    REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
            ("balance", "500.000 UTXO")
            ("claimed",1)
    );

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "quantity exceeds available supply" ),
            issue( N(alice), N(alice), asset::from_string("500.001 UTXO"), "hola" )
    );

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "must issue positive quantity" ),
            issue( N(alice), N(alice), asset::from_string("-1.000 UTXO"), "hola" )
    );

    BOOST_REQUIRE_EQUAL( success(),
            issue( N(alice), N(alice), asset::from_string("1.000 UTXO"), "hola" )
    );


} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( transfer_tests, xst_utxo_tester ) try {

    auto token = create( N(alice), asset::from_string("1000 UTXO"));
    produce_blocks(1);

    issue( N(alice), N(alice), asset::from_string("1000 UTXO"), "hola" );

    auto stats = get_stats("0,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "1000 UTXO")
            ("max_supply", "1000 UTXO")
            ("issuer", "alice")
    );

    auto alice_balance = get_account(N(alice), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
            ("balance", "1000 UTXO")
            ("claimed",1)
    );

    transfer( N(alice), N(bob), asset::from_string("300 UTXO"), "hola" );

    alice_balance = get_account(N(alice), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
            ("balance", "700 UTXO")
            ("claimed",1)
            ("frozen", 0)
            ("whitelist", 1)
    );

    auto bob_balance = get_account(N(bob), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( bob_balance, mvo()
            ("balance", "300 UTXO")
            ("claimed",0)
            ("frozen", 0)
            ("whitelist", 1)
    );

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "overdrawn balance" ),
            transfer( N(alice), N(bob), asset::from_string("701 UTXO"), "hola" )
    );

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "must transfer positive quantity" ),
            transfer( N(alice), N(bob), asset::from_string("-1000 UTXO"), "hola" )
    );


} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( loadutxo_tests, xst_utxo_tester ) try {

    create( N(alice), asset::from_string("1000 UTXO"));
    produce_blocks(1);

    issue( N(alice), N(alice), asset::from_string("1000 UTXO"), "hola" );
    produce_blocks(1);

    auto stats = get_stats("0,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
        ("supply", "1000 UTXO")
        ("max_supply", "1000 UTXO")
        ("issuer", "alice")
    );

    auto alice_balance = get_account(N(alice), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
        ("balance", "1000 UTXO")
        ("claimed",1)
    );

    transfer( N(alice), N(bob), asset::from_string("300 UTXO"), "hola" );

    alice_balance = get_account(N(alice), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
        ("balance", "700 UTXO")
        ("claimed",1)
        ("frozen", 0)
        ("whitelist", 1)
    );

    auto bob_balance = get_account(N(bob), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( bob_balance, mvo()
        ("balance", "300 UTXO")
        ("claimed",0)
        ("frozen", 0)
        ("whitelist", 1)
    );

    public_key_type public_key = get_public_key(N(bob), "owner");

    auto token = loadutxo( N(bob), public_key, asset::from_string("100 UTXO"));

    bob_balance = get_account(N(bob), "0,UTXO");
    REQUIRE_MATCHING_OBJECT( bob_balance, mvo()
            ("balance", "300 UTXO")
            ("claimed",0)
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( transferutxo_tests, xst_utxo_tester ) try {

    create( N(alice), asset::from_string("1000.0000 UTXO"));
    produce_blocks(1);

    issue( N(alice), N(alice), asset::from_string("1000.0000 UTXO"), "hola" );

    auto stats = get_stats("4,UTXO");
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("supply", "1000.0000 UTXO")
            ("max_supply", "1000.0000 UTXO")
            ("issuer", "alice")
    );

    produce_blocks(1);

    signed_transaction trx;
    set_transaction_headers(trx);

    transfer( N(alice), N(bob), asset::from_string("300.0000 UTXO"), "hola" );

    public_key_type bob_public_key = get_public_key(N(bob), "active");
    private_key_type bob_private_key = get_private_key(N(bob), "active");

    public_key_type carol_public_key = get_public_key(N(carol), "active");

    loadutxo( N(bob), bob_public_key, asset::from_string("100.0000 UTXO"));

    auto sig = trx.sign(bob_private_key, control->get_chain_id());

    std::vector<input> inputs;
    input in;
    in.id = 0;
    in.sig = sig;
    inputs.push_back(in);

    std::vector<output> outputs;
    output out;
    out.pk = carol_public_key;
    out.account = N(carol);
    out.quantity = asset::from_string("50.0000 UTXO");
    outputs.push_back(out);

    auto token = transferutxo(N(bob), inputs, outputs,"hola");

    auto carol_utxo_balance = get_utxo(N(carol_public_key), "4,UTXO");

//    REQUIRE_MATCHING_OBJECT( carol_utxo_balance, mvo()
//            ("amount", "50.0000 UTXO")
//    );
    produce_blocks(1);

} FC_LOG_AND_RETHROW()


BOOST_AUTO_TEST_SUITE_END()