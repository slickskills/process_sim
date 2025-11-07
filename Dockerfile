FROM debian:12-slim
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
	apt-get install -y --no-install-recommends \
		build-essential \
		cmake \
		git \
		libomp-dev \
		gfortran \
		binutils \
		ca-certificates \
	&& rm -rf /var/lib/apt/lists/*
WORKDIR /usr/src/simulator
CMD ["/bin/bash"]
