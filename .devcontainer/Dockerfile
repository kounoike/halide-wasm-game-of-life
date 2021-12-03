FROM ghcr.io/kounoike/imgproc-wasm:latest

USER vscode
RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
ENV PATH="/home/vscode/.cargo/bin:${PATH}"
RUN cargo install twiggy 
RUN cargo install wasm-nm
