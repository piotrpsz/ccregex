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
You need to download two libraries (if you don't have them yet):
1. Qt - to install on macOS, run the command `brew install qt6`
2. PCRE2 - to install on macOS, run the command`brew install pcre2`

If someone does not have (or does not want to have) the PCRE2 library on their computer, they should modify the CMakeLists.txt file.<br> 
In line 2 of this file, change ON to OFF. Currently it is _**set(PCRE2 ON)**_. Or comment out this line.

The program allows you to test several patterns with several source texts at the same time (every pattern with every source text).
Of course, we use this opportunity if it makes sense.<br>
Patterns and sources can be saved to a file on disk and read from them (see app menu).

![Screenshot.png](img%2FScreenshot.png)
