# Comparison of Linux sandboxing mechanisms

Prepared by:

​	Emil Latypov, e.latypov@innopolis.university<br>	Azamat Shakirov, a.shakirov@innopolis.university<br>	Khabib Khaysadykov, k.khaysadykov@innopolis.university 

## Introduction 

Nowadays there are a plenty of attack vectors in cyberspace which are used by intruders. Each operating system has made one or several attempts to minimize these attack vectors and provide the ability to safely use the system for its users. As a result of such attempts in Linux community there appeared several mechanisms which implement security measures in different scopes. Such techniques as `namespaces`, `cgroups` and `seccomp` provide mechanisms to minimize impact of running untrusted code. Despite separately they are not able to completely protect system from attacks, together they form powerful sandboxing set of measures which can significantly reduce possibility to harm or compromise system to attackers. Nevertheless, there also exist mechanism which are not supposed to serve security requirements and just seem to be so. As examples of such mechanisms we can take the example of `LD_PRELOAD` and `chroot`. Unfortunately, some developers inspired by elegance of this techniques falsely assume that they can use these mechanisms in order to implement security measures. Our research aims to compare different Linux mechanisms which are seem to provide sandboxing and to reveal unusable ones. Moreover, we made an attempt to review each mechanisms, provide usage examples and measure performance of the solutions.

## Methodology

For testing reasons we implemented several simple apps:

1. `suspicious_app` - application, which simulates some suspicious activity, such as opening and reading files, reading list of processes in the system and performing some network activity. It is supposed be used to prove that some mechanism is able to restrict or control its activity in order to protect system from untrusted code execution.
2. `stress_app` - application, which provides some load for `cpu` in user space and kernel space. It is supposed to be used in order to measure mechanism overhead comparing to running bare unprotected code.

Each mechanism has the following sections:

1. **Review** - short explanation of mechanism, vector of attacks it is supposed to protect from, advantages and disadvantages
2. **Demonstration** - mechanism usage examples with code provided and testing instructions. Here techniques are tested on `suspicous_app` if applicable
3. **Performance** - overhead measurements. Here `stress_app` is used if applicable
4. **Conclusion** - our contemplations on technology, final verdict on applicability
5. **References** - useful links

## Content

- [seccomp](./mechanisms/seccomp/Readme.md)

- [cgroups](./mechanisms/cgroups/Readme.md)

- [namespaces](./mechanisms/namespaces/Readme.md)

- [LD_PRELOAD](./mechanisms/LD_PRELOAD/Readme.md)

- [chroot](./mechanisms/chroot/Readme.md)
