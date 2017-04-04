@cd ..

cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G "MinGW Makefiles"
@if errorlevel 1 (
	echo Failed, aborting commit.
@	exit /b %errorlevel%
) else (
	echo Cmake passed, continuing with make.
)

mingw32-make
@if errorlevel 1 (
	echo Failed, aborting commit.
@	exit /b %errorlevel%
) else (
	echo Make passed, continuing with test.
)

@cd bin
kyrys_test.exe
@if errorlevel 1 (
	echo Failed, aborting commit.
@	exit /b %errorlevel%
) else (
	echo Tests passed, continuing with cppcheck.
)

@cd ..
cppcheck --enable=all --project=compile_commands.json
@if errorlevel 1 (
	echo Failed, aborting commit.
@	exit /b %errorlevel%
) else (
	echo Check passed, continuing with commit.
)

@exit 0
