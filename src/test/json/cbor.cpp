// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"
#include "test_unhex.hpp"

#include <tao/json.hpp>

namespace tao
{
   namespace json
   {
      void cbor_encode( const std::string& text, const std::string& data )
      {
         TEST_ASSERT( cbor::to_string( from_string( text ) ) == test_unhex( data ) );
      }

      void cbor_decode( const std::string& data, const std::string& text )
      {
         TEST_ASSERT( to_string( cbor::from_string( test_unhex( data ) ) ) == to_string( from_string( text ) ) );
      }

      void cbor_roundtrip( const std::string& text )
      {
         const auto a = from_string( text );
         TEST_ASSERT( to_string( a ) == to_string( cbor::from_string( cbor::to_string( a ) ) ) );
      }

      void unit_test()
      {
         cbor_encode( "null", "f6" );
         cbor_encode( "true", "f5" );
         cbor_encode( "false", "f4" );

         cbor_encode( "0", "00" );
         cbor_encode( "1", "01" );
         cbor_encode( "23", "17" );
         cbor_encode( "24", "1818" );
         cbor_encode( "255", "18ff" );
         cbor_encode( "256", "190100" );
         cbor_encode( "65535", "19ffff" );
         cbor_encode( "65536", "1a00010000" );
         cbor_encode( "4294967295", "1affffffff" );
         cbor_encode( "4294967296", "1b0000000100000000" );
         cbor_encode( "18446744073709551615", "1bffffffffffffffff" );

         cbor_encode( "\"0\"", "6130" );

         cbor_encode( '"' + std::string( std::size_t( 23 ), 'D' ) + '"', "77" + std::string( std::size_t( 46 ), '4' ) );
         cbor_encode( '"' + std::string( std::size_t( 24 ), 'D' ) + '"', "7818" + std::string( std::size_t( 48 ), '4' ) );
         cbor_encode( '"' + std::string( std::size_t( 255 ), 'D' ) + '"', "78ff" + std::string( std::size_t( 510 ), '4' ) );
         cbor_encode( '"' + std::string( std::size_t( 256 ), 'D' ) + '"', "790100" + std::string( std::size_t( 512 ), '4' ) );
         cbor_encode( '"' + std::string( std::size_t( 65535 ), 'D' ) + '"', "79ffff" + std::string( std::size_t( 131070 ), '4' ) );
         cbor_encode( '"' + std::string( std::size_t( 65536 ), 'D' ) + '"', "7a00010000" + std::string( std::size_t( 131072 ), '4' ) );
         //         cbor_encode( '"' + std::string( std::size_t( 4294967295 ), 'D' ) + '"', "7affffffff" + std::string( std::size_t( 8589934590 ), '4' ) );  // Uses 24GB of RAM.
         //         cbor_encode( '"' + std::string( std::size_t( 4294967296 ), 'D' ) + '"', "7b0000000100000000" + std::string( std::size_t( 8589934592 ), '4' ) );  // Uses 24GB of RAM.

         cbor_encode( "[]", "80" );
         cbor_encode( "{}", "a0" );

         cbor_encode( "[1,2,3,4]", "8401020304" );
         cbor_encode( "{\"a\":0,\"b\":1}", "a2616100616201" );

         cbor_encode( "[{}]", "81a0" );

         cbor_decode( "00", "0" );
         cbor_decode( "80", "[]" );
         cbor_decode( "9fff", "[]" );
         cbor_decode( "8100", "[0]" );
         cbor_decode( "9f00ff", "[0]" );
         cbor_decode( "8180", "[[]]" );
         cbor_decode( "9f80ff", "[[]]" );
         cbor_decode( "9f9fffff", "[[]]" );
         cbor_decode( "819fff", "[[]]" );

         cbor_decode( "f90400", "0.00006103515625" );
         cbor_decode( "f9c400", "-4.0" );

         cbor_decode( "8a00010203040506070809", "[0,1,2,3,4,5,6,7,8,9]" );
         cbor_decode( "9f00010203040506070809ff", "[0,1,2,3,4,5,6,7,8,9]" );
         cbor_decode( "a0", "{}" );
         cbor_decode( "bfff", "{}" );
         cbor_decode( "a1616100", "{\"a\":0}" );
         cbor_decode( "bf616100ff", "{\"a\":0}" );
         cbor_decode( "7f6161626262ff", "\"abb\"" );

         // TODO: Decode tests for broken inputs.

         cbor_roundtrip( "0" );
         cbor_roundtrip( "1" );
         cbor_roundtrip( "-1" );
         cbor_roundtrip( "24" );
         cbor_roundtrip( "257" );
         cbor_roundtrip( "-129" );
         cbor_roundtrip( "65536" );
         cbor_roundtrip( "-32769" );
         cbor_roundtrip( "4294967296" );
         cbor_roundtrip( "-2147483649" );
         cbor_roundtrip( "null" );
         cbor_roundtrip( "true" );
         cbor_roundtrip( "false" );
         cbor_roundtrip( "[]" );
         cbor_roundtrip( "{}" );
         cbor_roundtrip( "[{}]" );
         cbor_roundtrip( "[1,2,3,4]" );
         cbor_roundtrip( "{\"a\":[],\"b\":{}}" );
         cbor_roundtrip( "1.23" );
         cbor_roundtrip( "345e-123" );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
