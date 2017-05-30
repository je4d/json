// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_JSON_INTERNAL_TEMPORARY_HPP
#define TAOCPP_JSON_INCLUDE_JSON_INTERNAL_TEMPORARY_HPP

#include <cstdint>
#include <cstring>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // TODO: Where to put this header?
         // TODO: Support big-endian platforms (degrade to a nop).
         // TODO: Support other compilers (VS: intrin.h's _byteswap_ushort etc.)

         template< unsigned S > struct bswap;

         template<> struct bswap< 1 >
         {
            static std::uint8_t convert( const std::uint8_t n ) noexcept
            {
               return n;
            }
         };

         template<> struct bswap< 2 >
         {
            static std::uint16_t convert( const std::uint16_t n ) noexcept
            {
               return __builtin_bswap16( n );
            }
         };

         template<> struct bswap< 4 >
         {
            static double convert( float n ) noexcept
            {
               std::uint32_t u;
               std::memcpy( &u, &n, 4 );
               u = convert( u );
               std::memcpy( &n, &u, 4 );
               return n;
            }

            static std::uint32_t convert( const std::uint32_t n ) noexcept
            {
               return __builtin_bswap32( n );
            }
         };

         template<> struct bswap< 8 >
         {
            static double convert( double n ) noexcept
            {
               std::uint64_t u;
               std::memcpy( &u, &n, 8 );
               u = convert( u );
               std::memcpy( &n, &u, 8 );
               return n;
            }

            static std::uint64_t convert( const std::uint64_t n ) noexcept
            {
               return __builtin_bswap64( n );
            }
         };

         template< typename N >
         N h_to_be( const N n ) noexcept
         {
            return N( bswap< sizeof( N ) >::convert( n ) );
         }

         template< typename N >
         N be_to_h( const N n ) noexcept
         {
            return h_to_be( n );
         }

         template< typename N >
         N be_to_h( const void* p ) noexcept
         {
            N n;
            std::memcpy( &n, p, sizeof( n ) );
            return internal::be_to_h( n );
         }

      } // internal

   } // namespace json

} // namespace tao

#endif
