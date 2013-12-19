if (errno = 0, (key = ftok("/grad/users/morbius", 42) == -1 )
else if (errno = 0, (shmid = shmget(key, size O666, | IPC_REAT | IPC_EXCL)) != -1)
else if (errno = 0, (shmid = shmget(key, 0, O666) ) != -1)
