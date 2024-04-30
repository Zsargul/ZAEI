#ifndef WM_SETUP_H
#define WM_SETUP_H

int install_dwm(const char* repoUrl, const char* targetDir);
int clone_repo(const char* name, const char* repoUrl, const char* targetDir);
int repo_exists(const char* repoUrl);

#endif
