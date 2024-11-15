{
  description = "Cringe dev environment (it's C dude)";

  inputs = { };

  outputs = { self, nixpkgs } @ inputs:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.${system}.default = with pkgs; mkShell rec {
        packages = [
          gcc
          gdb
          lldb
          bear
          clang
          gnumake
          clang-tools
          nixpkgs-fmt
          pkg-config
        ];
        buildInputs = [
          gcc
          clang
        ];
        LD_LIBRARY_PATH = lib.makeLibraryPath buildInputs;
      };
    };
}
