all:
	@echo "[git] Pulling files"
	@git pull #get the latest version of the code if we don't already have it
	@echo "[git] Adding all files to push"
	@git add *
	@echo "[git] Committing all new changes"
	@git commit -m "Update"
	@echo "[git] Pushing new changes."
	@git push