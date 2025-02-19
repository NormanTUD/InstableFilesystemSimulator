#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>

static ssize_t (*real_read)(int, void *, size_t) = NULL;
static ssize_t (*real_write)(int, const void *, size_t) = NULL;
static int (*real_open)(const char *, int, ...) = NULL;
static int (*real_close)(int) = NULL;
static int (*real_fsync)(int) = NULL;

// Umgebungsvariablen für Fehlerwahrscheinlichkeit und Verzögerung
#define ENV_VAR_ERROR_PROBABILITY "FS_ERROR_PROBABILITY"
#define ENV_VAR_MAX_DELAY_MS "FS_MAX_DELAY_MS"

// Default-Werte
#define DEFAULT_ERROR_PROBABILITY 5  // 20% Fehlerchance
#define DEFAULT_MAX_DELAY_MS 1000  // max. 1 Sekunden Verzögerung

int ERROR_PROBABILITY = DEFAULT_ERROR_PROBABILITY;
int MAX_DELAY_MS = DEFAULT_MAX_DELAY_MS;

void init_real_functions() {
	if (!real_read) real_read = dlsym(RTLD_NEXT, "read");
	if (!real_write) real_write = dlsym(RTLD_NEXT, "write");
	if (!real_open) real_open = dlsym(RTLD_NEXT, "open");
	if (!real_close) real_close = dlsym(RTLD_NEXT, "close");
	if (!real_fsync) real_fsync = dlsym(RTLD_NEXT, "fsync");

	// Umgebungsvariablen auslesen und anpassen
	const char *error_prob_env = getenv(ENV_VAR_ERROR_PROBABILITY);
	if (error_prob_env) {
		ERROR_PROBABILITY = atoi(error_prob_env);
	}

	const char *max_delay_env = getenv(ENV_VAR_MAX_DELAY_MS);
	if (max_delay_env) {
		MAX_DELAY_MS = atoi(max_delay_env);
	}

	srand(time(NULL));  // Seed für Zufallswerte
}

// Zufällige Verzögerung basierend auf Umgebungsvariablen
#define RANDOM_DELAY usleep((rand() % MAX_DELAY_MS) * 1000)

// Manipuliertes read
ssize_t read(int fd, void *buf, size_t count) {
	init_real_functions();

	if (rand() % ERROR_PROBABILITY == 0) {  // Fehlerchance
		errno = EIO;
		return -1;
	}

	RANDOM_DELAY;
	return real_read(fd, buf, count);
}

// Manipuliertes write
ssize_t write(int fd, const void *buf, size_t count) {
	init_real_functions();

	if (rand() % ERROR_PROBABILITY == 0) {
		errno = EIO;
		return -1;
	}

	if (rand() % 10 == 0) {  // 10% Chance, dass wir zufällige Daten schreiben
		((char *)buf)[0] = rand() % 256;
	}

	RANDOM_DELAY;
	return real_write(fd, buf, count);
}

// Manipuliertes open
int open(const char *pathname, int flags, ...) {
	init_real_functions();

	if (rand() % ERROR_PROBABILITY == 0) {  // 20% Chance auf ENOENT oder EPERM
		errno = (rand() % 2) ? ENOENT : EPERM;
		return -1;
	}

	RANDOM_DELAY;
	return real_open(pathname, flags);
}

// Manipuliertes close
int close(int fd) {
	init_real_functions();

	if (rand() % 15 == 0) {  // 7% Chance auf EBADF
		errno = EBADF;
		return -1;
	}

	RANDOM_DELAY;
	return real_close(fd);
}

// Manipuliertes fsync
int fsync(int fd) {
	init_real_functions();

	if (rand() % 8 == 0) {  // 12.5% Fehlerchance
		errno = EIO;
		return -1;
	}

	RANDOM_DELAY;
	return real_fsync(fd);
}
