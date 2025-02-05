The licensing information in the documentation for previous versions of this software is out-of-date.

You MUST read the licensing information accompanying this source code before using it.  Licensing information can be found in “LICENSE.TXT” or in the main source file “mainwindow.cpp”.  This software is licensed under the “X11 License”.  

Suggestions for improvement from the original author:

Concerning compiling under Visual Studio 7.0
-This source should compile with no (or very very few) changes under Visual C++ 7.0

-It is recommended that the STL string library be used instead of the “bstring” library.  The two are source-level compatible but the STL library is optimized better.  This is done by unincluding “bstring” and including “<string>” (not “string.h”) and then using the namespace “std”.

-It is recommended that the STL vector library be used instead of the “bvect” library.  The two libraries are very close to source-level compatible (you will need to write/alter remove and insert to use STL) and with some source modification STL may be used and should be because it is optimized better.  You will need to uninclude “bvect” and include “<vector>” and then use the namespace “std”.

Concerning speeding up execution time
-“Translhextion::print_line” is a major source of execution slow-down, the first optimizations should be done here

-The translation functions in “table.h” are another source of execution slow-down, optimizations may also be done here

-Using STL under VC 7.0 instead of “bstring” and “bvect” as mentioned in the above note should increase speed for many operations

Unimplemented requests from users
-Greater than 3byte values (i.e. 4 for psx)
-Number the lines on script output
-Recalculation of pointers
-Section break (to accompany existing newline and endstring values)