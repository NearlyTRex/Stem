/*
  Copyright (c) 2010 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __RANDOM__
#define __RANDOM__

/** Sets random seed. Note that the RANROT implementation used is somewhat nonrandom for several
    iterations after seeding; for better randomness, you may want to "stir" the PRNG by calling
    irand() some number of times after seeding and discarding the generated values. */
void sdrand(int seed);

/** Returns a random integer uniformly distributed between 0 and INT_MAX. For a number between 0 and
    UINT_MAX, call irand() and cast the result to unsigned int. */
unsigned int uirand(void);

/** Returns a random integer uniformly distributed between INT_MIN and INT_MAX */
int irand(void);

/** Returns a random float uniformly distributed between 0.0f and range */
float ufrand(float range);

/** Returns a random float uniformly distributed between -range and range */
float frand(float range);

#endif
