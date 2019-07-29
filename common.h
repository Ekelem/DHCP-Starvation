#ifndef _HEADER_COMMON_
#define _HEADER_COMMON_

/*
 Computer Communications and Networks 2017/2018
 Project n. 2, variant 2 (DHCP Starvation attack)

 Autor: Erik Kelemen
*/


/** macro bit_mask
 * @brief create mask of ones on interval <from,to>
 * 
 * @param from beggining, must be less than 'to'
 * @param to end, must be more than 'from'
 * 
 * @return mask
 */

#define bit_mask(from, to) (((1 << (to - from)) - 1) << from)

#endif //_HEADER_COMMON_
