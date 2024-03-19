# ccregex - regular expressions in C++.
Program for testing regular expressions using several engines available in C++. <br>

Currently, you can test regular expressions using the C++ standard library (std). <br>
In the near future you will be able to test regular expressions with engines:
<lu>
    <li>[Qt RegularExpression](https://doc.qt.io/qt-6/qregularexpression.html)</li>
    <li>[PCRE2](https://github.com/PCRE2Project/pcre2)</li>
</lu>

The program itself uses the Qt 6.x library as a GUI.<br>
I created a program on macOS using CMake.<br>

### You need to download three libraries (if you don't have them yet):
1. Qt - to install on macOS, run the command `brew install qt6`
2. PCRE2 - to install on macOS, run the command `brew install pcre2`
3. fmt - to install on macOS, run the command `brew install fmt`

If someone does not have (or does not want to have) the PCRE2 library on their computer, they should modify the CMakeLists.txt file.<br> 
In line 2 of this file, change ON to OFF. Currently it is `set(PCRE2 ON)`. Or comment out this line.

The program allows you to test several patterns with several source texts at the same time (every pattern with every source text).
Of course, we use this opportunity if it makes sense.<br>
Patterns and sources can be saved to a file on disk and read from them (see app menu).

### How to build the program
1. open a terminal and navigate to the directory of your choice
2. clone the program from github
3. `cd ccregex` go to ccregex directory,
4. `mkdir build`
5. `cd build`
6. `cmake ..`
7. `cmake --build .`
8. (if you would like to build from beginning, remove previous content: `cmake --fresh .` and go to point 7.)
9. `./ccregex` (run)


![Screenshot.png](img%2FScreenshot.png)

### Versions 
<lu>
    <li>0.0.1 - initial working version</li>
    <li>0.1.0 - it is possible to work with several documents at the same time</li>
</lu>
