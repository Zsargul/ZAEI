#ifndef GITUTILS_H
#define GITUTILS_H

int git_installed();
int clone_repo(const char* name, const char* repoUrl, const char* targetDir);

#endif
