# Namespaces

## Review

Namespaces in Linux are a feature that allow different processes to have different views of the system resources, such as the file system, network interfaces, and process IDs. Each namespace provides an isolated environment for processes, which can be useful for various reasons, such as security, resource isolation, and management.

For example, the network namespace can be used to create an isolated network stack for a group of processes. This namespace provides the illusion of a completely separate networking system for each isolated group of processes, with its own set of network interfaces, routing tables, and firewall rules. This allows different groups of processes to communicate with each other over their own, isolated virtual network. Similarly, the PID namespace allows each process to have its own unique set of process IDs, which can be useful for process isolation.

Linux provides several different types of namespaces, including mount, network, PID, user, IPC, and UTS namespaces [1]. These namespaces provide different types of isolation and can be combined to create more complex isolation environments.

**Mount Namespace**

The mount namespace provides a separate view of the file system hierarchy for a group of processes. This means that processes within a mount namespace can mount or unmount file systems without affecting the rest of the system.

When a new mount namespace is created, it starts out as a clone of the parent namespace. From there, the processes within the new namespace can mount or unmount file systems as desired, and those mount points will only be visible to other processes within the same namespace. The parent namespace and any other namespaces running on the system will not see these mount points.

Each mount namespace is associated with a mount point, which serves as the root of the file system hierarchy within that namespace. When a process accesses the file system within the namespace, it does so relative to this mount point, which may be a virtual file system or a directory on a real file system.

The mount namespace is an important component of Linux container technology, as it allows containers to have their own isolated file systems. This makes it possible for a container to have its own set of libraries and other dependencies, without affecting the rest of the system.

**Network Namespace**


The network namespace provides a distinct network stack for a group of processes. This means that processes within a network namespace can have their own network interfaces, routing table, IP addresses, ports, and firewall rules, separate from the rest of the system.

When a new network namespace is created, it starts out as a clone of the parent namespace, with its own set of network interfaces. The processes running within the new namespace can configure the network interfaces as desired and apply routing tables, firewall rules, and other networking configurations. Any changes made within the namespace only affect the processes running within that namespace, and are not visible to processes in other namespaces.

Each network namespace has its own loopback interface, which enables processes within the namespace to communicate with each other through the network stack. Processes running within different network namespaces can communicate through virtual network interfaces that can be connected using networking tools like bridges.

The network namespace is an essential component of Linux containers, as it enables containers to have their own distinct network environment, just like they have their own file system. This concept of network namespace allows different containers to run on the same host and communicate with each other as if they were running on separate hosts, while still being isolated from the host’s default network environment.

**PID Namespace**


The PID namespace provides each process with its own isolated view of the system's process ID hierarchy. This means that processes within a PID namespace see only their own set of processes, and not those running in other namespaces.

When a new PID namespace is created, it starts out as a clone of the parent namespace. From there, processes within the new namespace can create and manage their own processes without affecting the rest of the system. Processes within the new namespace can see their own processes and those of any child namespaces, but not those of other unrelated namespaces.

Each PID namespace has a root process, typically with a PID of 1. This process is responsible for creating and managing other processes within the namespace. Other processes within the namespace are given unique PIDs relative to this root process.

The PID namespace is essential to the functioning of Linux containers, as it allows each container to have its own set of processes with distinct PIDs, while still running on the same system as other containers. This provides a high degree of process-level isolation, preventing processes within one container from interfering with those in another container or in the host system.

Overall, the PID namespace is a key component of the Linux kernel's namespace feature, providing a powerful tool for process isolation and management in complex, multi-process systems.

**User Namespace**

The user namespace provides a way to map user and group IDs inside a container or other namespace to a different set of user IDs on the host system. This allows processes within the container to run as root and access system resources that require root privileges, without actually having full root privileges on the host system.

When a new user namespace is created, it starts out as a clone of the parent namespace. From there, processes running inside the new namespace can be mapped to a different set of user and group IDs relative to the host system. This makes it possible for processes within the namespace to run as root and access system resources that require root privileges, without actually having full root privileges on the host system.

User namespaces are essential to the functioning of Linux containers, as they allow containers to run untrusted code or services while reducing the risk of privilege escalation attacks. This means that even if a malicious process within a container manages to break out of the container's file or network namespace, it won't be able to gain full root access to the host system.

Overall, the user namespace is a useful tool for managing privileges and ensuring security in multi-process systems such as Linux containers or virtual machines.

**IPC Namespace**

The IPC namespace provides each process with its own isolated view of the system's IPC resources, such as message queues, semaphores, and shared memory segments. This means that processes within an IPC namespace see only their own set of IPC resources, and not those used by processes in other namespaces.

When a new IPC namespace is created, it starts out as a clone of the parent namespace. From there, processes within the new namespace can create and access their own IPC resources without affecting the rest of the system. Any changes made within the namespace only affect the processes running within that namespace and are not visible to processes in other namespaces.

Each IPC namespace has its own set of IPC IDs used for message queues, semaphores, and shared memory segments. Processes within the namespace can create and manage their own IPC resources using these IDs, even if they duplicate IDs already in use by other processes outside the namespace.

The IPC namespace is essential to the functioning of Linux containers, as it allows each container to have its own set of IPC resources, preventing processes within one container from interfering with those in another container or in the host system. This provides a high degree of process-level isolation, ensuring that applications running within the container do not rely on system resources that could be accessed by other processes.

Overall, the IPC namespace is a key component in the Linux kernel's namespace feature, providing a powerful tool for process isolation and management in complex, multi-process systems.

**UTS Namespace**

The UTS namespace provides each process with its own isolated view of the system's hostname and NIS (Network Information Service) domain name. This means that processes within a UTS namespace see only their own hostname and domain name, and not those of other processes in other namespaces.

When a new UTS namespace is created, it starts out as a clone of the parent namespace. From there, processes within the new namespace can set and modify their own hostname and domain name without affecting the rest of the system. Any changes made within the namespace only affect the processes running within that namespace and are not visible to processes in other namespaces.

Each UTS namespace has its own hostname and domain name, which can be set and modified by processes within the namespace using the sethostname() and setdomainname() system calls.

The UTS namespace is essential to the functioning of Linux containers, as it allows each container to have its own hostname and domain name, preventing processes within one container from interfering with those of another container or the host system. This provides a high degree of process-level isolation, ensuring that applications running within the container do not rely on system resources that could be accessed by other processes.

Overall, the UTS namespace is a key component in the Linux kernel's namespace feature, providing a powerful tool for process isolation and management in complex, multi-process systems.

## Demonstration



Some text and code examples or/and instructions. If mechanism can be violated, add examples how to violate it.

## Performance

If reasonable, measure mechanism performance and explain here methodology. Write measurement results. Interested in overhead with and without applying this mechanism.

## Conclusion

Overall, Linux namespaces are a powerful tool for creating isolated environments for processes and can be used to improve security, resource management, and other aspects of system management. However, they require a good understanding of Linux system internals and can be difficult to get right, so careful consideration and planning are recommended before deploying them in production environments.

## References

[1] Borate, Imamjafar & Chavan, Raosaheb. (2016). Sandboxing in Linux: From Smartphone to Cloud. International Journal of Computer Applications. 148. 1-8. 10.5120/ijca2016911256. 