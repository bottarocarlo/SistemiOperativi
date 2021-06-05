#!/bin/bash
docker run -ti --rm --name="labOS" --privileged -v /:/host -v "$(pwd):/home/labOS" --hostname "labOS" --workdir /home/labOS ubuntu:20.04 /bin/bash 
