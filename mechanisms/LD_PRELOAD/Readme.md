# LD_PRELOAD

## Review

`LD_PRELOAD` is a Linux environment variable that is often used to preload a dynamic library before executing a program. Theoretically, it can be used in sandboxing purposes by overriding some critical standard library function calls, such as `open`, `read`, `connect` or other calls which imply use of system resources. By setting `LD_PRELOAD` to a custom shared object, developers can replace system library functions with custom ones that are designed to block certain actions or monitor them for suspicious behavior. For instance, it is possible to replace `open` call with custom implementation which always blocks file open operation. This can be useful in testing untrusted code or limiting the impact of a compromised system.

However, it's important to note that `LD_PRELOAD` is not a foolproof security mechanism and can be bypassed by certain techniques. It is easy to overpass such "sandbox" just by compiling target application statically instead of linking it dynamically with the shared library. In this case all standard library functions are build directly into the binary and preload trick will not work.

## Demonstration

We implemented simple shared library `fake_libc.so` which overrides several standard library functions, such as `open`, `opendir` and `connect`. New versions of this functions always return error signal according to function specification (e.g. `open` always returns `-1`, `opendir` always returns NULL, etc.).

Here is an example of stub for `open`:

```C
int open(const char *pathname, int flags) {
    return -1;
}
```

As we can see, it always returns error value. Other function can be found in `mechanisms/LD_PRELOAD/fake_libc.c` from project root.

We prepared 2 tests for this mechanism: the first one is to check with dynamically linked library and the second one is to check is statically linked library.

First, build our shared library:

```bash
make -C mechanisms/LD_PRELOAD/
```

Then run test on dynamically linked binary:

```bash
make -C mechanisms/LD_PRELOAD/ test_dynamic 
```

As a result we can observe that all suspicious activities failed:

```
=== read_file start ===

Failed to open /etc/passwd

=== read_file fail ===

=== count_processes start ===

Failed to open /proc directory

=== count_processes fail ===

=== access_network start ===

Failed to connect

=== access_network fail ===


Summary: 0 succeed, 3 failed
```



Let's try to run tests on statically linked library:

```bash
make -C mechanisms/LD_PRELOAD/ test_static
```

Despite of overriding standard library functions with custom implementations, all suspicious activities were performed succcessfully:

```
=== read_file start ===

Opened /etc/passwd
First line of /etc/passwd:
root:x:0:0:root:/root:/bin/bash

=== read_file success ===

=== count_processes start ===

332 processes detected

=== count_processes success ===

=== access_network start ===

Connected to server successfuly

=== access_network success ===


Summary: 3 succeed, 0 failed
```

> Note: as `LD_PRELOAD` is just environment variable, it can be used in the following way:
>
> ```bash
> LD_PRELOAD=PATH_TO_SHARED PATH_TO_BINARY
> ```
>
> Further details can be found in `mechanisms/LD_PRELOAD/Makefile` 

## Performance

`LD_PRELOAD` does not add any extra overhead  on top of loading an additional shared library into memory. However, if  we use `LD_PRELOAD` to dynamically load the original function using `dlopen` (which sometimes can be the case)  and wrap it with your own implementation, there will be some  overhead. This is because explicitly loading the original function adds  some extra work and can slow down the performance of the application. Keep in mind that it is important to weigh the advantages of using `LD_PRELOAD` and overwriting functions against the potential impact on performance.

## Conclusion

As a conclusion, `LD_PRELOAD` is useful `linux` mechanism which can be applied on injecting custom code into the application. But, it is ABSOLUTELY NOT APPLICABLE as a sandboxing mechanism in terms of cybersecurity because it is very easy to bypass it. `LD_PRELOAD`  is not imply being sandboxing solution, it is used in other areas. It is reasonable to refuse using `LD_PRELOAD` as a sandboxing mechanism and take a look towards other ones, such as `seccomp` and `namespaces`.

<img src="../../images/negative_checkbox.svg" width="150" />

# References
