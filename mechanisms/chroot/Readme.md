# Chroot

## Review
`Chroot` is an operation that enables the current running process and its child processes to perceive a different root directory. When a program is executed within this modified environment, it is confined to accessing only the files and commands within the designated directory tree. This confinement creates a restricted environment known as a `chroot jail`, preventing access to resources outside of it.

It's important to note that while chroot provides some level of isolation, it is not a foolproof security measure. It is still possible for processes to escape the chroot environment if they have vulnerabilities or if the chroot environment is not properly set up.

## Demonstration

To create an isolated environment for running potentially suspicious applications, we obtain a `rootfs` (https://cdimage.ubuntu.com/ubuntu-base/releases/22.04/release/), which serves as the foundation. This `rootfs` consists of a foundational Ubuntu version that includes all the essential files needed to execute our program securely within this confined environment. We will execute a series of commands to set up the sandbox environment. 

```
cp -r ./suspicious_app ~/sandbox
sudo chroot ~/sandbox
```

To prepare the sandbox environment, we copy the contents of the `suspicious_app` directory to a new directory called `sandbox`. Then, we use `chroot` to set `sandbox` as the new root directory, confining subsequent commands and processes to operate within the sandboxed environment. By running our suspicious application in this sandboxed environment, we can observe the resulting outcome.

```
=== read_file start ===

Failed to open /etc/passwd

=== read_file fail ===

=== count_processes start ===

0 processes detected

=== count_processes fail ===

=== access_network start ===

Connected to server successfuly

=== access_network success ===


Summary: 1 succeed, 2 failed
```

While running the suspicious application in the sandboxed environment, we observed the following outcomes: the attempt to read the file /etc/passwd was unsuccessful, no processes were found during the process count, and there was a successful connection made during network access. However, it's important to note that chroot alone does not provide limitations on network access.

Now, we will proceed to develop a C program that aims to escape the chroot environment, and subsequently, we will test its effectiveness.

```
  mkdir("chroot-dir", 0755);
  chroot("chroot-dir");
  for(int i = 0; i < 1000; i++) {
    chdir("..");
  }
  chroot(".");
  system("/bin/bash");
```
The program mentioned above will execute the following actions: It will set up a chroot environment, change the directory to a location outside the chroot (thus enabling access to the root file system beyond the chroot boundaries), and enter the chroot to gain entry into the root file system. Now, we will proceed to execute our suspicious program once more and observe the resulting outcomes.

```
=== read_file start ===

Opened /etc/passwd
First line of /etc/passwd:
root:x:0:0:root:/root:/bin/bash

=== read_file success ===

=== count_processes start ===

323 processes detected

=== count_processes success ===

=== access_network start ===

Connected to server successfuly

=== access_network success ===


Summary: 3 succeed, 0 failed

```
## Performance 

In terms of performance, the `chroot` mechanism itself has minimal overhead on system resources as changing the root directory is a lightweight operation. However, the performance of applications within the chroot environment may be affected by limited access to resources outside the designated directory tree. Confinement within the `chroot jail` can restrict access to files, commands, and system resources beyond the chroot, potentially impacting application performance, particularly for those heavily reliant on external resources. Additionally, the additional steps in the provided code, such as navigating outside the chroot directory using `chdir("..")`, may introduce slight overhead due to system calls and directory traversal.

## Conclusion

Considering the `chroot` mechanism for sandboxing purposes in terms of cybersecurity, we can draw certain conclusions. The mechanism, while providing some level of isolation, is not an acceptable solution for robust sandboxing due to its inherent vulnerabilities. The demonstrated ability to break out of the chroot environment using simple techniques indicates that relying solely on chroot for security is inadequate.

To achieve a higher level of cybersecurity, it is essential to explore more advanced sandboxing solutions that offer stronger isolation and confinement capabilities. These solutions should be designed to mitigate the vulnerabilities associated with chroot and provide enhanced protection against potential threats.

<img src="../../images/negative_checkbox.svg" width="150" />