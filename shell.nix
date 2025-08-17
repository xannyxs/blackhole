with import <nixpkgs> { };

mkShell {
  buildInputs = [
    gcc
    gnumake
    pkg-config
    clang-tools
    bear

    glfw
    glm

    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    xorg.libXfixes
  ];
}
