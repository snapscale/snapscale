#pragma once
#include <eoslib/message.h>
#include <eoslib/print.hpp>

namespace eos {

   /**
    * @defgroup messagecppapi Message C++ API
    * @ingroup messageapi
    * @brief Type-safe C++ wrapers for Message C API
    *
    * @note There are some methods from the @ref messagecapi that can be used directly from C++
    *
    * @{
    */

   /**
    *  This method attempts to reinterpret the message body as type T. This will only work
    *  if the message has no dynamic fields and the struct packing on type T is properly defined.
    *  @brief Interpret the message body as type T
    *  Example
    *  @code
    *
    *  struct MyMessage {
    *     uint32_t time;
    *  };
    *
    *  MyMessage msg = currentMessage<MyMessage>();
    *  @endcode
    */
   template<typename T>
   T currentMessage() {
      T value;
      auto read = readMessage( &value, sizeof(value) );
      assert( read >= sizeof(value), "message shorter than expected" );
      return value;
   }

   using ::requireAuth;
   using ::requireNotice;

   /**
    *  All of the listed accounts must be specified on the message notice list or this method will throw
    *  and end execution of the message.
    *
    *  This helper method enables you to require notice of multiple accounts with a single
    *  call rather than having to call the similar C API multiple times.
    *
    *  @note message.code is also considered as part of the set of notified accounts
    *
    *  @brief Verify specified accounts exist in the set of notified accounts
    *  Example
    *  @code
    *  requireNotice(N(Account1), N(Account2), N(Account3)); // throws exception if any of them not in set.
    *  @endcode
    */
   template<typename... Accounts>
   void requireNotice( AccountName name, Accounts... accounts ){
      requireNotice( name );
      requireNotice( accounts... );
   }


 ///@} messagecpp api

} // namespace eos
