#ifndef GITUTILS_H
#define GITUTILS_H

int git_installed();
int clone_repo(const char* repoUrl, const char* targetDir);
int repo_exists(const char* repoUrl);

#endif
