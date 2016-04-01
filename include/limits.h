/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    partial limits.h
    
    George Robertson
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/




#ifndef _LIMITS_H_
#define _LIMITS_H_

#ifndef __CHAR_BIT_DEFINED__

    #define __CHAR_BIT_DEFINED__
    #if defined(__CHAR_BIT__)
        #define CHAR_BIT __CHAR_BIT__
    #else
        #define CHAR_BIT 8
        #warning "__CHAR_BIT__ is not defined, defaulting CHAR_BIT to 8"
    #endif

#endif


#endif /* _LIMITS_H_ */
