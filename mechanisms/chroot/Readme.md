# Chroot

## Review
chroot (change root) is a Unix command that allows you to change the root directory for a process and its children to a different location in the filesystem. It creates a virtualized environment where the specified directory becomes the new root directory, and any access to files and directories outside of that directory is restricted.

Chroot is commonly used for sandboxing or isolating processes. By setting up a chroot environment, you can create a restricted environment in which you can execute applications or processes without risking the security or stability of the main system.

It's important to note that while chroot provides some level of isolation, it is not a foolproof security measure. It is still possible for processes to escape the chroot environment if they have vulnerabilities or if the chroot environment is not properly set up.

## Demonstration

To create an isolated environment for running potentially suspicious applications, we obtain a rootfs (https://cdimage.ubuntu.com/ubuntu-base/releases/22.04/release/), which serves as the foundation. This rootfs consists of a foundational Ubuntu version that includes all the essential files needed to execute our program securely within this confined environment. We will execute a series of commands to set up the sandbox environment. 

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

In order to use `chroot`, we initially set up a sandbox environment using an `Ubuntu base`, which served as the testing environment. We proceeded to test the suspicious application within this environment and obtained satisfactory results. However, we subsequently demonstrated that it is relatively simple to break out of the sandbox environment and execute the suspicious application. The results clearly indicated that we were able to successfully break out of the sandbox environment.

## Conclusion

When evaluating the `chroot` mechanism for sandboxing purposes in terms of cybersecurity, certain conclusions can be drawn. Unfortunately, the mechanism does not provide an acceptable level of security due to its inherent vulnerabilities. The isolated environment created by chroot is relatively easy to break, making it unreliable as a robust security measure. It is important to explore alternative sandboxing solutions that offer stronger isolation and confinement capabilities to ensure a higher level of cybersecurity for protecting against potential threats.

<img src="../../images/negative_checkbox.svg" width="150" />
