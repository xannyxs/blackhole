with import <nixpkgs> { };

mkShell {
  buildInputs = [
    gcc
    gnumake
    pkg-config
    glfw
    clang-tools
    bear

    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    xorg.libXfixes
  ];
}
