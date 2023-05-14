all:
	gcc seccomp_sandbox.c -o seccomp_sandbox -lseccomp

test_dynamic:
	make -C ../../suspicious_app
	./seccomp_sandbox ../../suspicious_app/app

test_static:
	make -C ../../suspicious_app
	./seccomp_sandbox ../../suspicious_app/app_static

clean:
	rm -f seccomp_sandbox