#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/asset.hpp>

namespace eosio { namespace chain {

    class txfee_manager {
    
    public:
        explicit txfee_manager();
    };

    class fee_paramter {
     public:
       account_name name;
       asset fee;
       account_name producer;
       account_name mec;
       fee_paramter(account_name name, asset fee, account_name producer, account_name mec) : name(name), fee(fee), producer(producer), mec(mec) {};
   };
} } /// namespace eosio::chain

FC_REFLECT(eosio::chain::fee_paramter, (name)(fee)(producer)(mec))