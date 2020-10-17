{ lib, rustPlatform, fetchFromGitHub, writeText }:

rustPlatform.buildRustPackage rec {
    name = "blog_os-sysroot-test";

    src = fetchFromGitHub {
        owner = "phil-opp";
        repo = "blog_os";
        rev = "4e38e7ddf8dd021c3cd7e4609dfa01afb827797b";
        sha256 = "0k9ipm9ddm1bad7bs7368wzzp6xwrhyfzfpckdax54l4ffqwljcg";
    };

    cargoSha256 = "1cbcplgz28yxshyrp2krp1jphbrcqdw6wxx3rry91p7hiqyibd30";

    # The book uses rust-lld for linking, but rust-lld is not currently packaged for NixOS.
    # The justification in the book for using rust-lld suggests that gcc can still be used for testing:
    # > Instead of using the platform's default linker (which might not support Linux targets),
    # > we use the cross platform LLD linker that is shipped with Rust for linking our kernel.
    # https://github.com/phil-opp/blog_os/blame/7212ffaa8383122b1eb07fe1854814f99d2e1af4/blog/content/second-edition/posts/02-minimal-rust-kernel/index.md#L157
    target = writeText "x86_64-blog_os.json" ''
        {
            "llvm-target": "x86_64-unknown-none",
            "data-layout": "e-m:e-i64:64-f80:128-n8:16:32:64-S128",
            "arch": "x86_64",
            "target-endian": "little",
            "target-pointer-width": "64",
            "target-c-int-width": "32",
            "os": "none",
            "executables": true,
            "linker-flavor": "gcc",
            "panic-strategy": "abort",
            "disable-redzone": true,
            "features": "-mmx,-sse,+soft-float"
        }
    '';

    RUSTFLAGS = "-C link-arg=-nostartfiles";

    # Tests don't work for `no_std`. See https://os.phil-opp.com/testing/
    doCheck = false;

    meta = with lib; {
        description = "Test for using custom sysroots with buildRustPackage";
        maintainers = with maintainers; [ aaronjanse ];
    };
}
