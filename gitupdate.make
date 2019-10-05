all:
	git pull #get the latest version of the code if we don't already have it
	git add *
	git push
	git commit -m "Update"
	git push