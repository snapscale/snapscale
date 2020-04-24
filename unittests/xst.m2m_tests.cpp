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

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace fc;
using namespace std;

using mvo = fc::mutable_variant_object;

class xst_m2m_tester : public tester {
public:

    xst_m2m_tester() {
        produce_blocks( 2 );

        create_accounts( { N(usecasecarx1), N(usecasepile1), N(communityv1), N(xst.m2m) } ); // N(eosio.token)
        produce_blocks( 2 );

        //deploy xst.m2m
        set_code( N(xst.m2m), contracts::xst_m2m_wasm() );
        set_abi( N(xst.m2m), contracts::xst_m2m_abi().data() );

        produce_blocks();
        const auto& accnt = control->db().get<account_object,by_name>( N(xst.m2m) );
        abi_def abi;
        BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
        abi_ser.set_abi(abi, abi_serializer_max_time);

        //deploy eosio.token & create XST
        eosio_token_deploy();

    }

    //xst.m2m push action
    action_result push_action( const account_name& signer, const action_name &name, const variant_object &data ) {
        string action_type_name = abi_ser.get_action_type(name);
        action act;
        act.account = N(xst.m2m);
        act.name    = name;
        act.data    = abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

        return base_tester::push_action( std::move(act), signer.to_uint64_t());
    }

    //xst.m2m get stats
    fc::variant get_stats( account_name car )
    {//uint64_t code, uint64_t scope, uint64_t table, const account_name& act
        vector<char> data = get_row_by_account( N(xst.m2m), N(xst.m2m), N(carauthmsg), car );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "carauthmsg", data, abi_serializer_max_time );
    }


    action_result carauth( account_name car, account_name pile, asset quantity ) {
        return push_action( car, N(carauth), mvo()
                ( "car", car)
                ( "pile", pile)
                ( "quantity", quantity)
        );
    }

    action_result pilepersub( account_name pile, account_name car ) {
        return push_action( pile, N(pilepersub), mvo()
                ( "pile", pile)
                ( "car", car)
        );
    }

    action_result settlement( account_name pile, account_name car, account_name proxy ) {
        return push_action( pile, N(settlement), mvo()
                ( "pile", pile)
                ( "car", car)
                ( "proxy", proxy)
        );
    }

    abi_serializer abi_ser;
    //---------------------------------------------------------

    action_result create( account_name issuer,
                          asset        maximum_supply ) {
        return push_action_token( N(eosio.token), N(create), mvo()
                ( "issuer", issuer)
                ( "maximum_supply", maximum_supply)
        );
    }

    action_result issue( account_name issuer, account_name to, asset quantity, string memo ) {
        return push_action_token( issuer, N(issue), mvo()
                ( "to", to)
                ( "quantity", quantity)
                ( "memo", memo)
        );
    }

    action_result transfer( account_name from,
                            account_name to,
                            asset        quantity,
                            string       memo ) {
        return push_action_token( from, N(transfer), mvo()
                ( "from", from)
                ( "to", to)
                ( "quantity", quantity)
                ( "memo", memo)
        );
    }

    fc::variant get_stats_token( const string& symbolname )
    {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(eosio.token), name(symbol_code), N(stat), name(symbol_code) );
        return data.empty() ? fc::variant() : abi_ser_token.binary_to_variant( "currency_stats", data, abi_serializer_max_time );
    }

    fc::variant get_account_token( account_name acc, const string& symbolname)
    {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(eosio.token), acc, N(accounts), name(symbol_code) );
        return data.empty() ? fc::variant() : abi_ser_token.binary_to_variant( "account", data, abi_serializer_max_time );
    }

    action_result push_action_token( const account_name& signer, const action_name &name, const variant_object &data ) {
        string action_type_name = abi_ser_token.get_action_type(name);

        action act;
        act.account = N(eosio.token);
        act.name    = name;
        act.data    = abi_ser_token.variant_to_binary( action_type_name, data, abi_serializer_max_time );

        return base_tester::push_action( std::move(act), signer.to_uint64_t());
    }

    abi_serializer abi_ser_token;

    void eosio_token_deploy() {
        produce_blocks( 2 );

        create_accounts( {  N(eosio.token) } );
        produce_blocks( 2 );

        set_code( N(eosio.token), contracts::eosio_token_wasm() );
        set_abi( N(eosio.token), contracts::eosio_token_abi().data() );

        produce_blocks();

        const auto& accnt = control->db().get<account_object,by_name>( N(eosio.token) );
        abi_def abi;
        BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
        abi_ser_token.set_abi(abi, abi_serializer_max_time);

        /////

        auto token = create( N(usecasecarx1), asset::from_string("1000.0000 XST"));
        produce_blocks(2);

        issue( N(usecasecarx1), N(usecasecarx1), asset::from_string("1000.0000 XST"), "hola" );
        produce_blocks(2);

        auto stats = get_stats_token("4,XST");
        REQUIRE_MATCHING_OBJECT( stats, mvo()
                ("supply", "1000.0000 XST")
                        ("max_supply", "1000.0000 XST")
                        ("issuer", "usecasecarx1")
        );

        auto alice_balance = get_account_token(N(usecasecarx1), "4,XST");
        REQUIRE_MATCHING_OBJECT( alice_balance, mvo()
                ("balance", "1000.0000 XST")
        );
    }
};

BOOST_AUTO_TEST_SUITE(xst_m2m_tests)

    BOOST_FIXTURE_TEST_CASE( carauth_tests, xst_m2m_tester ) try {
        //add eosio.code premiss
        auto auth = authority(get_public_key(N(usecasecarx1), "active"));
        auth.accounts.push_back( permission_level_weight{{N(xst.m2m), config::eosio_code_name}, 1} );
        base_tester::push_action( N(eosio), N(updateauth), N(usecasecarx1), mvo()
                ( "account", "usecasecarx1" )
                ( "permission", "active" )
                ( "parent", "owner" )
                ( "auth", auth )
        );

        auto token = carauth( N(usecasecarx1), N(usecasepile1), asset::from_string("10.0000 XST"));
        auto stats = get_stats(N(usecasecarx1));
        REQUIRE_MATCHING_OBJECT( stats, mvo()
                ("car", "usecasecarx1")
                        ("pile", "usecasepile1")
                        ("quantity", "10.0000 XST")
                        ("comsume", "0.0000 XST")
        );
        produce_blocks(1);

    } FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( transfer_negative_quantity, xst_m2m_tester ) try {
    //add eosio.code premiss
    auto auth = authority(get_public_key(N(usecasecarx1), "active"));
    auth.accounts.push_back( permission_level_weight{{N(xst.m2m), config::eosio_code_name}, 1} );
    base_tester::push_action( N(eosio), N(updateauth), N(usecasecarx1), mvo()
            ( "account", "usecasecarx1" )
            ( "permission", "active" )
            ( "parent", "owner" )
            ( "auth", auth )
    );
    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "car must transfer positive quantity" ),
          carauth( N(usecasecarx1), N(usecasepile1), asset::from_string("-10.0000 XST"))
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( pilepersub_tests, xst_m2m_tester ) try {
    //add eosio.code premiss
    auto auth = authority(get_public_key(N(usecasecarx1), "active"));
    auth.accounts.push_back( permission_level_weight{{N(xst.m2m), config::eosio_code_name}, 1} );
    base_tester::push_action( N(eosio), N(updateauth), N(usecasecarx1), mvo()
            ( "account", "usecasecarx1" )
            ( "permission", "active" )
            ( "parent", "owner" )
            ( "auth", auth )
    );

    auto token = carauth( N(usecasecarx1), N(usecasepile1), asset::from_string("10.0000 XST"));
    produce_blocks(1);
    pilepersub(N(usecasepile1), N(usecasecarx1));
    auto stats = get_stats(N(usecasecarx1));
    REQUIRE_MATCHING_OBJECT( stats, mvo()
            ("car", "usecasecarx1")
            ("pile", "usecasepile1")
            ("quantity", "10.0000 XST")
            ("comsume", "1.0000 XST")
    );
    produce_blocks(1);

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( exceeding_collateral_token, xst_m2m_tester ) try {
    //add eosio.code premiss
    auto auth = authority(get_public_key(N(usecasecarx1), "active"));
    auth.accounts.push_back( permission_level_weight{{N(xst.m2m), config::eosio_code_name}, 1} );
    base_tester::push_action( N(eosio), N(updateauth), N(usecasecarx1), mvo()
            ( "account", "usecasecarx1" )
            ( "permission", "active" )
            ( "parent", "owner" )
            ( "auth", auth )
    );

    auto token = carauth( N(usecasecarx1), N(usecasepile1), asset::from_string("10.0000 XST"));
    produce_blocks(1);

    int time = 0;
    while(time < 10){
        pilepersub(N(usecasepile1), N(usecasecarx1));
        ++time;
    }
    produce_blocks(1);

    BOOST_REQUIRE_EQUAL( wasm_assert_msg( "[pilepersub] Insufficient balance, less than FIX_PERSUB" ),
        pilepersub(N(usecasepile1), N(usecasecarx1))
    );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( settlement_tests, xst_m2m_tester ) try {
    //add eosio.code premiss
    auto auth = authority(get_public_key(N(usecasecarx1), "active"));
    auth.accounts.push_back( permission_level_weight{{N(xst.m2m), config::eosio_code_name}, 1} );
    base_tester::push_action( N(eosio), N(updateauth), N(usecasecarx1), mvo()
            ( "account", "usecasecarx1" )
            ( "permission", "active" )
            ( "parent", "owner" )
            ( "auth", auth )
    );

    auto token = carauth( N(usecasecarx1), N(usecasepile1), asset::from_string("10.0000 XST"));
    produce_blocks(1);

    pilepersub(N(usecasepile1), N(usecasecarx1));
    produce_blocks(1);

    settlement(N(usecasepile1), N(usecasecarx1), N(communityv1));

    auto usecasecarx1_balance = get_account_token(N(usecasecarx1), "4,XST");
    REQUIRE_MATCHING_OBJECT( usecasecarx1_balance, mvo()
            ("balance", "999.0000 XST")
    );
    auto usecasepile1_balance = get_account_token(N(usecasepile1), "4,XST");
    REQUIRE_MATCHING_OBJECT( usecasepile1_balance, mvo()
            ("balance", "0.5000 XST")
    );
    auto communityv1_balance = get_account_token(N(communityv1), "4,XST");
    REQUIRE_MATCHING_OBJECT( communityv1_balance, mvo()
            ("balance", "0.5000 XST")
    );
    produce_blocks(1);


} FC_LOG_AND_RETHROW()


BOOST_AUTO_TEST_SUITE_END()

