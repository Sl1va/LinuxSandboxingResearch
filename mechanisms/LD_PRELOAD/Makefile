all:
	gcc -shared -fPIC fake_libc.c -o fake_libc.so

test_dynamic:
	make -C ../../suspicious_app
	LD_PRELOAD=./fake_libc.so ../../suspicious_app/app

test_static:
	make -C ../../suspicious_app
	LD_PRELOAD=./fake_libc.so ../../suspicious_app/app_static

clean:
	rm -f fake_libc.so