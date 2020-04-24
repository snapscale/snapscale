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
#include <eosio/chain/config.hpp>
#include <eosio/chain/webassembly/common.hpp>
#include <eosio/chain/name.hpp>

#include <fc/crypto/digest.hpp>
#include <fc/crypto/sha256.hpp>
//#include <fc/crypto/private_key.hpp>
#include <fc/exception/exception.hpp>
#include <fc/variant_object.hpp>
#include <time.h>
#include <fc/crypto/aes.hpp>

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace fc;
using namespace fc::ecc;
using namespace std;
using namespace fc::crypto;

using mvo = fc::mutable_variant_object;


class xst_ca_tester : public tester {
public:

    xst_ca_tester() {
        produce_blocks( 2 );

        create_accounts( { N(alice), N(bob), N(carol), N(xst.ca) } );
        produce_blocks( 2 );

        set_code( N(xst.ca), contracts::xst_ca_wasm() );
        set_abi( N(xst.ca), contracts::xst_ca_abi().data() );

        produce_blocks();

        const auto& accnt = control->db().get<account_object,by_name>( N(xst.ca) );
        abi_def abi;
        BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
        abi_ser.set_abi(abi, abi_serializer_max_time);
    }

    action_result push_action( const account_name& signer, const action_name &name, const variant_object &data ) {
        string action_type_name = abi_ser.get_action_type(name);

        action act;
        act.account = N(xst.ca);
        act.name    = name;
        act.data    = abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

        return base_tester::push_action( std::move(act), signer.to_uint64_t());
    }

    action_result addmem( account_name owner,
                          public_key_type pk,
                          std::string meminfo
                                            ) {
        return push_action( N(xst.ca), N(addmem), mvo()
                ( "owner", owner)
                ( "pk", pk)
                ( "meminfo", meminfo)
        );
    }

    action_result addcert(public_key_type pk,
                          signature_type cert) {
        return push_action( N(xst.ca), N(addcert), mvo()
                ( "pk", pk)
                ( "cert", cert)
        );
    }

    action_result addcrl(public_key_type pk) {
        return push_action( N(xst.ca), N(addcrl), mvo()
                ( "pk", pk)
        );
    }

    action_result delmem(public_key_type pk) {
        return push_action( N(xst.ca), N(delmem), mvo()
                ( "pk", pk)
        );
    }

    action_result delcrl(public_key_type pk) {
        return push_action( N(xst.ca), N(delcrl), mvo()
                ( "pk", pk)
        );
    }

    fc::variant get_mem( const uint64_t id)
    {
        vector<char> data = get_row_by_account( N(xst.ca), N(xst.ca), N(members), name(id) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "member", data, abi_serializer_max_time );
    }

    fc::variant get_cert( const uint64_t id)
    {
        vector<char> data = get_row_by_account( N(xst.ca), N(xst.ca), N(certs), name(id) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "certinfo", data, abi_serializer_max_time );
    }

    fc::variant get_crl( const uint64_t id)
    {
        vector<char> data = get_row_by_account( N(xst.ca), N(xst.ca), N(crls), name(id) );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "crl", data, abi_serializer_max_time );
    }

//    fc::variant get_mem( const public_key_type pk)
//    {
//        vector<char> data = get_row_by_account( N(xst.token), N(xst.token), N(memebers), pk );
//        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "member", data, abi_serializer_max_time );
//    }
//
//    fc::variant get_certinfo( const public_key_type pk)
//    {
//        vector<char> data = get_row_by_account( N(xst.token), N(xst.token), N(certs), pk );
//        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "certinfo", data, abi_serializer_max_time );
//    }
//
//    fc::variant get_crl( const public_key_type pk)
//    {
//        vector<char> data = get_row_by_account( N(xst.token), N(xst.token), N(crls), pk );
//        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "crl", data, abi_serializer_max_time );
//    }

    void initial_params(public_key_type& rootPk,
                        public_key_type& peerAPk,
                        public_key_type& peerBPk,
                        public_key_type& peerCPk,
                        public_key_type& peerDPk,
                        public_key_type& peerEPk,
                        signature_type& certA,
                        signature_type& certB,
                        signature_type& certC,
                        signature_type& certD,
                        signature_type& certE)
    {
        //cert generate
        auto rootPrivS = std::string("5KF5jtD8QGKpypuT3HLDYjDiVTzrLvkYiEb2XXtxwabty87m9ib");
        auto rootPriv = fc::crypto::private_key(rootPrivS);
        rootPk = rootPriv.get_public_key();

        auto peerAS = std::string("5J75daXKsAESNuPffb7kxeHsGXZmAKDbzSsWrR2tQyzTcuwu5SY");
        auto peerAPriv = fc::crypto::private_key(peerAS);
        peerAPk = peerAPriv.get_public_key();
        sha256 peerAPk256 = sha256::hash((char*)&peerAPk, sizeof(fc::ecc::public_key_data));
        auto peerAPkS = peerAPk.to_string();

        auto peerBS = std::string("5K8jggGd1iEqHVUXCHd3z3uvFHgCXvQbveRrM3wKDp3YvLL5f2T");
        auto peerBPriv = fc::crypto::private_key(peerBS);
        peerBPk = peerBPriv.get_public_key();
        sha256 peerBPk256 = sha256::hash((char*)&peerBPk, sizeof(fc::ecc::public_key_data));
        auto peerBPkS = peerBPk.to_string();


        auto peerCS = std::string("5JkKF55rJ6ugBhYUREzCn4G781B1Qrom1f7CdJiGEvHax5vTaBP");
        auto peerCPriv = fc::crypto::private_key(peerCS);
        peerCPk = peerCPriv.get_public_key();
        sha256 peerCPk256 = sha256::hash((char*)&peerCPk, sizeof(fc::ecc::public_key_data));
        auto peerCPkS = peerCPk.to_string();


        auto peerDS = std::string("5Jqdas2n6Ta6F6DeYNgQsy8y3RqfGna22H4Juguu8KfS4efa5X9");
        auto peerDPriv = fc::crypto::private_key(peerDS);
        peerDPk = peerDPriv.get_public_key();
        sha256 peerDPk256 = sha256::hash((char*)&peerDPk, sizeof(fc::ecc::public_key_data));
        auto peerDPkS = peerDPk.to_string();


        auto peerES = std::string("5K2UKNYxwr152sB3Q6nfJdbp43oU2F8CQdV1CpCS8V8W7y9SPYt");
        auto peerEPriv = fc::crypto::private_key(peerES);
        peerEPk = peerEPriv.get_public_key();
        sha256 peerEPk256 = sha256::hash((char*)&peerEPk, sizeof(fc::ecc::public_key_data));
        auto peerEPkS = peerEPk.to_string();


        certA = rootPriv.sign(peerAPk256);
        certB = rootPriv.sign(peerBPk256);
        certC = rootPriv.sign(peerCPk256);
        certD = rootPriv.sign(peerDPk256);
        certE = rootPriv.sign(peerEPk256);

        produce_blocks(1);
    }

    abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(xst_ca_tests)

    BOOST_FIXTURE_TEST_CASE( addmem_tests, xst_ca_tester ) try {

        public_key_type rootPk;
        public_key_type peerAPk;
        public_key_type peerBPk;
        public_key_type peerCPk;
        public_key_type peerDPk;
        public_key_type peerEPk;
        signature_type certA;
        signature_type certB;
        signature_type certC;
        signature_type certD;
        signature_type certE;
        initial_params(rootPk, peerAPk, peerBPk, peerCPk, peerDPk, peerEPk, certA, certB, certC, certD, certE);
        produce_blocks(1);

        auto peerAPkS = peerAPk.to_string();
        auto certAS = certA.to_string();

        auto peerBPkS = peerBPk.to_string();
        auto certBS = certB.to_string();

        sha256 peerAPk256 = sha256::hash((char*)&peerAPk, sizeof(fc::ecc::public_key_data));
//        auto recovered_pub = public_key(certA, peerAPk256);

        auto rootPkS = rootPk.to_string();

        auto rec = fc::crypto::public_key(certA, peerAPk256);
        auto recS = rec.to_string();

        sha256 peerBPk256 = sha256::hash((char*)&peerBPk, sizeof(fc::ecc::public_key_data));


        auto recB = fc::crypto::public_key(certB, peerBPk256);
        auto recBS = recB.to_string();

        auto sigA = std::string("SIG_K1_KeCZD8yzhR4pCvt3RwDemiPubEMWRk336zDaNJNa5r5vnWxZ3QRFMQt7XAj69hnLS5QQadtGWzy2hUsxu32kLm4nFRceQq");
        signature_type sig(sigA);
        auto recA = fc::crypto::public_key(sig, peerAPk256).to_string();



        addmem(N(alice), peerAPk, "test:test,test1:test1");

        produce_blocks(1);




        auto member = get_mem(0);
//        REQUIRE_MATCHING_OBJECT( member, mvo()
//            ("id", 0)
//            ("status", 0)
//            ("owner",N(alice))
//            ("pk",peerAPk)
//            ("meminfo", "test:test,test1:test1")
//        );


    } FC_LOG_AND_RETHROW()


    BOOST_FIXTURE_TEST_CASE( addcert_tests, xst_ca_tester ) try {

        public_key_type rootPk;
        public_key_type peerAPk;
        public_key_type peerBPk;
        public_key_type peerCPk;
        public_key_type peerDPk;
        public_key_type peerEPk;
        signature_type certA;
        signature_type certB;
        signature_type certC;
        signature_type certD;
        signature_type certE;
        initial_params(rootPk, peerAPk, peerBPk, peerCPk, peerDPk, peerEPk, certA, certB, certC, certD, certE);

        auto certAS = certA.to_string();
        addcert(peerAPk, certA);
        addcert(peerBPk, certB);
        addcert(peerCPk, certC);
        addcert(peerDPk, certD);

        produce_blocks(1);
        auto cert = get_cert(0);
//        REQUIRE_MATCHING_OBJECT( cert, mvo()
//          ("id",0)
//          ("pk",peerAPk)
//          ("cert", certA)
//        );

    } FC_LOG_AND_RETHROW()
//
    BOOST_FIXTURE_TEST_CASE( addcrl_tests, xst_ca_tester ) try {
        public_key_type pubkey(std::string("XST6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV")); // private generate key, not UT's generate key 
        addcrl(pubkey);
        {
            auto crl = get_crl(0);
            REQUIRE_MATCHING_OBJECT( crl, mvo()
                    ("id",0)
                    ("pk",pubkey)
            );
        }

        public_key_type rootPk;
        public_key_type peerAPk;
        public_key_type peerBPk;
        public_key_type peerCPk;
        public_key_type peerDPk;
        public_key_type peerEPk;
        signature_type certA;
        signature_type certB;
        signature_type certC;
        signature_type certD;
        signature_type certE;
        initial_params(rootPk, peerAPk, peerBPk, peerCPk, peerDPk, peerEPk, certA, certB, certC, certD, certE);

        addcrl(peerAPk);
        addcrl(peerBPk);
        addcrl(peerCPk);
        addcrl(peerDPk);
        addcrl(peerEPk);
        produce_blocks(1);


        // auto crl = get_crl(0);
//        REQUIRE_MATCHING_OBJECT( crl, mvo()
//                ("pk",peerAPk)
//                ("cert", certA)
//        );

    } FC_LOG_AND_RETHROW()

    BOOST_FIXTURE_TEST_CASE( delmem_tests, xst_ca_tester ) try {

        public_key_type rootPk;
        public_key_type peerAPk;
        public_key_type peerBPk;
        public_key_type peerCPk;
        public_key_type peerDPk;
        public_key_type peerEPk;
        signature_type certA;
        signature_type certB;
        signature_type certC;
        signature_type certD;
        signature_type certE;
        initial_params(rootPk, peerAPk, peerBPk, peerCPk, peerDPk, peerEPk, certA, certB, certC, certD, certE);

        addcert(peerAPk, certA);
        addcert(peerBPk, certB);

        produce_blocks(1);

        delmem(peerAPk);

        //To do


    } FC_LOG_AND_RETHROW()

    BOOST_FIXTURE_TEST_CASE( delcrl_tests, xst_ca_tester ) try {

        public_key_type rootPk;
        public_key_type peerAPk;
        public_key_type peerBPk;
        public_key_type peerCPk;
        public_key_type peerDPk;
        public_key_type peerEPk;
        signature_type certA;
        signature_type certB;
        signature_type certC;
        signature_type certD;
        signature_type certE;
        initial_params(rootPk, peerAPk, peerBPk, peerCPk, peerDPk, peerEPk, certA, certB, certC, certD, certE);

        addcrl(peerAPk);
        addcrl(peerBPk);

        produce_blocks(1);

        delcrl(peerAPk);

        //To do

    } FC_LOG_AND_RETHROW()




BOOST_AUTO_TEST_SUITE_END()

