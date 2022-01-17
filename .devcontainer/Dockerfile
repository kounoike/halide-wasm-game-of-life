FROM ghcr.io/kounoike/imgproc-wasm:latest

RUN apt-get update && apt-get install -y libappindicator1 fonts-liberation libnss3 libdrm2 libgbm1 xdg-utils firefox
RUN curl -O https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb && \
  dpkg -i google-chrome-stable_current_amd64.deb && \
  rm google-chrome-stable_current_amd64.deb

USER vscode
RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
ENV PATH="/home/vscode/.cargo/bin:${PATH}"
RUN cargo install twiggy 
RUN cargo install wasm-nm

RUN echo '. /emsdk/emsdk_env.sh > /dev/null 2>&1' >> /home/vscode/.bashrc
