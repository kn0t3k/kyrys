What's Kyrys project
---
Kyrys is small terminal app used for P2P encrypted chat between 2 clients. We are developing this project for purpose not to create second [WhatsApp](https://www.whatsapp.com/), but to learn something about secure coding, communication through internet and application of cryptography theory into real project.

---

Automatic build status:

* TravisCI: ![Build Status](https://travis-ci.org/kn0t3k/kyrys.svg?branch=master)

---

Study materials:

* [Private study materials FI MUNI](https://is.muni.cz/auth/www/410316/68411172/)
* [Qt Network Documentation](http://doc.qt.io/qt-5/qtnetwork-programming.html)
* [mbedTLS Documentation](https://tls.mbed.org/kb)

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
