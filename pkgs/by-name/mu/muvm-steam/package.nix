{
  bashInteractive,
  config,
  lib,
  muvm,
  path,
  writeShellApplication,
  writeShellScript,
}:

let
  pkgsx86_64 = import path {
    system = "x86_64-linux";
    inherit config;
  };

  inherit (pkgsx86_64) steam mesa;
  mesa32 = pkgsx86_64.pkgsi686Linux.mesa;

  initScript = writeShellScript "muvm-steam-init.sh" ''
    ln -snf ${mesa} /run/opengl-driver
    ln -snf ${mesa32} /run/opengl-driver-32
    ln -s ${steam} /run/steam
    echo "Run steam with /run/steam/bin/steam"
    echo
  '';
in

writeShellApplication {
  name = "muvm-steam";
  text = ''
    ${lib.getExe muvm} -x ${initScript} "''${SHELL:-${lib.getExe bashInteractive}}"
  '';
}
