all:
	gcc namespaces_sandbox.c -o namespaces_sandbox

test_dynamic:
	make -C ../../suspicious_app
	sudo ./namespaces_sandbox ../../suspicious_app/app

test_static:
	make -C ../../suspicious_app
	./namespaces_sandbox ../../suspicious_app/app_static

clean:
	rm -f namespaces_sandbox