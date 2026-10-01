/* Redirect Chrome's open of resources.pak to Papaya's patched copy.
 * Bubblewrap cannot do this: its user namespace ignores the setgid bit on
 * 1Password-BrowserSupport, so the desktop app cannot unlock the extension.
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

static const char TARGET[] = "/opt/google/chrome/resources.pak";
static const char *repl;
static int in_init;
static int (*real_open64)(const char *, int, ...);
static int (*real_open)(const char *, int, ...);
static int (*real_openat64)(int, const char *, int, ...);
static int (*real_openat)(int, const char *, int, ...);
static FILE *(*real_fopen64)(const char *, const char *);
static FILE *(*real_fopen)(const char *, const char *);

static int raw_openat(int dirfd, const char *path, int flags, int mode) {
  return (int)syscall(SYS_openat, dirfd, path, flags | O_LARGEFILE, mode);
}

static int take_mode(int flags, va_list ap) {
  if (flags & (O_CREAT | O_TMPFILE)) return va_arg(ap, int);
  return 0;
}

__attribute__((constructor)) static void papaya_init(void) {
  const char *env;
  in_init = 1;
  real_open64 = dlsym(RTLD_NEXT, "open64");
  real_open = dlsym(RTLD_NEXT, "open");
  real_openat64 = dlsym(RTLD_NEXT, "openat64");
  real_openat = dlsym(RTLD_NEXT, "openat");
  real_fopen64 = dlsym(RTLD_NEXT, "fopen64");
  real_fopen = dlsym(RTLD_NEXT, "fopen");
  env = getenv("PAPAYA_RESOURCES_PAK");
  if (env && access(env, R_OK) == 0) repl = strdup(env);
  in_init = 0;
}

static int dir_is_chrome(int dirfd) {
  char link[64];
  char buf[4096];
  ssize_t n;
  snprintf(link, sizeof link, "/proc/self/fd/%d", dirfd);
  n = readlink(link, buf, sizeof buf - 1);
  if (n < 0) return 0;
  while (n > 1 && buf[n - 1] == '/') n--;
  buf[n] = 0;
  return strcmp(buf, "/opt/google/chrome") == 0;
}

static const char *rewrite(int dirfd, const char *path) {
  if (!repl || !path) return path;
  if (path[0] == '/') return strcmp(path, TARGET) == 0 ? repl : path;
  if ((dirfd == AT_FDCWD || dir_is_chrome(dirfd)) && strcmp(path, "resources.pak") == 0)
    return repl;
  return path;
}

int open64(const char *path, int flags, ...) {
  int mode;
  va_list ap;
  va_start(ap, flags);
  mode = take_mode(flags, ap);
  va_end(ap);
  if (in_init || !real_open64) return raw_openat(AT_FDCWD, path, flags, mode);
  path = rewrite(AT_FDCWD, path);
  if (flags & (O_CREAT | O_TMPFILE)) return real_open64(path, flags, mode);
  return real_open64(path, flags);
}

int open(const char *path, int flags, ...) {
  int mode;
  va_list ap;
  va_start(ap, flags);
  mode = take_mode(flags, ap);
  va_end(ap);
  if (in_init || !real_open) return raw_openat(AT_FDCWD, path, flags, mode);
  path = rewrite(AT_FDCWD, path);
  if (flags & (O_CREAT | O_TMPFILE)) return real_open(path, flags, mode);
  return real_open(path, flags);
}

int openat64(int dirfd, const char *path, int flags, ...) {
  int mode;
  va_list ap;
  va_start(ap, flags);
  mode = take_mode(flags, ap);
  va_end(ap);
  if (in_init || !real_openat64) return raw_openat(dirfd, path, flags, mode);
  path = rewrite(dirfd, path);
  if (path && path[0] == '/' && strcmp(path, TARGET) != 0)
    dirfd = AT_FDCWD;
  if (flags & (O_CREAT | O_TMPFILE)) return real_openat64(dirfd, path, flags, mode);
  return real_openat64(dirfd, path, flags);
}

int openat(int dirfd, const char *path, int flags, ...) {
  int mode;
  va_list ap;
  va_start(ap, flags);
  mode = take_mode(flags, ap);
  va_end(ap);
  if (in_init || !real_openat) return raw_openat(dirfd, path, flags, mode);
  path = rewrite(dirfd, path);
  if (path && path[0] == '/' && real_openat) dirfd = AT_FDCWD;
  if (flags & (O_CREAT | O_TMPFILE)) return real_openat(dirfd, path, flags, mode);
  return real_openat(dirfd, path, flags);
}

FILE *fopen64(const char *path, const char *mode) {
  if (in_init || !real_fopen64) return NULL;
  path = rewrite(AT_FDCWD, path);
  return real_fopen64(path, mode);
}

FILE *fopen(const char *path, const char *mode) {
  if (in_init || !real_fopen) return NULL;
  path = rewrite(AT_FDCWD, path);
  return real_fopen(path, mode);
}
