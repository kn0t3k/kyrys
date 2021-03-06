What's Kyrys project
---
This project is no longer in development. Don't fork it, we still have some bugs in code that cause problems.
---
Kyrys is small terminal app used for P2P encrypted chat between 2 clients. We are developing this project for purpose not to create second [WhatsApp](https://www.whatsapp.com/), but to learn something about secure coding, communication through internet and application of cryptography theory into real project.

---

Oficial documentation:
* [Documentation and manual in CZ/SK languages for final review in subject PB173](https://github.com/kn0t3k/kyrys/tree/master/doc/Oficial%20documentation/SK-CZ%20manual)

---

Study materials:

* [Private study materials FI MUNI](https://is.muni.cz/auth/www/410316/68411172/)
* [Qt Network Documentation](http://doc.qt.io/qt-5/qtnetwork-programming.html)
* [mbedTLS Documentation](https://tls.mbed.org/kb)
* [Catch framework: multi-paradigm automated test framework for C++](https://github.com/philsquared/Catch)

---

Build:

* set QT directories in CMakeLists.txt
* run 'cmake . -G "MinGW Makefiles"
* run 'mingw32-make'
* enjoy Kyrys

* when commiting:
	- install cppcheck and add it to PATH
	- copy file 'pre-commit' from scripts/ to .git/hooks
	- after running 'git commit ...' a script will check if your changes will compile and if the tests will pass

How to start:
1. Build project
2. Create folder C:\__TEMP__
2.1. Create database files db.txt and dbTEST.txt in this folder. On windows, be careful to don't create file with double postfix *.txt.txt. This will not work.
3. Open commandline terminal and switch working directory to .../GIT/kyrys/bin and start server by starting binary file kyrysServer.exe
4. When server is online, open second terminal and start kyrysClient.exe from same directory as server.
5. Now you have online client and server and you can start using Kyrys project.

More info about using client or server you will get from documentation or by executing command "help" in client. 
