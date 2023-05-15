# Seccomp

## Review

`Seccomp` (abbreviation from **sec**ure **comp**uting) is a  Linux kernel technology that provides mechanism which may be a part of sandboxing a process in a Linux environment. The technique involves restricting the system calls that a process can make to only what is needed. The operating system can use this system call filtering mechanism to ensure that the calls are safe and approved. `Seccomp` has various applications for making applications secure like restricting system calls which can affect system resources, such as opening and writing to files, creating new processes etc.

The key advantage of `seccomp` is that it restricts the system calls used by a process to a permitted list, making it important for organizations that need to enforce information security controls. `Seccomp` reduces the attack vector of an application by restricting its access to the kernel's system functions. The mechanism is straightforward to implement and maintains reduced complexity in the applications' security policies.

## Demonstration

`Seccomp` can be activated using `prctl` system call, which is used to perform specials operations on the process. But there exists more flexible and convenient way to use this feature - `libseccomp`. We implemented simple application, which activates `seccomp` with several rules and executes target process.

To work with the file we need to install `libseccomp` and include its header in source file:

```c
#include <seccomp.h>
```

The whole setting up function has the following content:

```C
void set_up_seccomp(void) {
    /* Create seccomp context with allow-all mode */
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);

    /* Add `openat` and `connect` syscalls to blacklist */   
    seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(openat), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(connect), 0);

    /* Apply seccomp rules */
    seccomp_load(ctx);
    printf("Seccomp activated\n");
}
```

Let's consider each part of this code this code.

First, create `seccomp` context, which we want to apply to the current execution flow:

```C
/* Create seccomp context with allow-all mode */
scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
```

In the context creation we used `SCMP_ACT_ALLOW` identifier, which means that by default any system call is available. It is also possible to use identifier `SCMP_ACT_KILL`, which will immediately kill application if it tries to call not permitted system call. Moreover, there exists identifier `SCMP_ACT_ERRNO` which will force to fail not permitted system call execution but allow to continue application running just by returning error code from target system call and set `errno` to specified one. For example, we can set identifier to `SCMP_ACT_ERRNO(EPERM)`  which means permission error, and if we try to call some non-permitted system function, such as `open`, it will return error value (which is `-1` for `open`) and set `errno` to `EPERM`.



Then, add rules to the `seccomp` context:

```C
/* Add `openat` and `connect` syscalls to blacklist */   
seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(openat), 0);
seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(connect), 0);
```

As we created context with rule `SCMP_ACT_ALLOW` it will allow all system calls by default. Then we blocked `openat` and `connect` system calls by using `SCMP_ACT_ERRNO` rule, which is described above. These system calls are used by testing `suspicous_app` and blocking them will lead to failing its activities.

>Note: In order to simplify demonstration we used all-allowing `seccomp` rule by default and explicitly blocked several system calls. Nevertheless, in production cases it is highly recommended to avoid this architecture and block all system calls by default and  allow only necessary system calls instead. Moreover, it is also worth mentioning is that it is possible to add rules which allow some system calls with only specific arguments, e.g. allowing to call `read` only for standard input file descriptor (which is `1`) 



In order to apply `seccomp` rules to the current process it is required to call appropriate function explicitly:

```C
/* Apply seccomp rules */
seccomp_load(ctx);
```



To build `seccomp` sandbox use the following command:

```bash
make -C mechanisms/seccomp/
```

To test the solution run the following command:

```bash
make -C mechanisms/seccomp/ test_static
```

And see that all suspicious activities are failed:

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



`seccomp_sandbox` uses the following format:

```bash
./seccomp_sandbox TARGET_BINARY_PATH
```

It configures `seccomp` rules and performs `execve` to the target application.

## Performance

However, there are certain disadvantages to Seccomp. One of the issues that come with Seccomp is performance. Seccomp reduces a process's performance by limiting the system calls that can be made to a kernel. It is important to have an effective whitelist of system calls and then optimize the policy. The system may encounter issues when running existing applications that require newer system calls or when unexpected behavior occurs. These issues arise when the application does not run inside a sandbox and may encounter problems shifting to restricted privileges as used by Seccomp.

## Conclusion

To sum up, Seccomp is an essential tool for application security and system hardening, and is relevant in use-cases where security is the utmost priority and optimal system performance isn't the top priority. Even with Seccomp enforced, it is still important to follow security best practices, such as avoiding the use of hardcoded credentials and making sure to patch and update the system frequently to minimize vulnerabilities.

<img src="../../images/positive_checkbox.svg" width="100" />

## References
