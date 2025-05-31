import hashlib
import io
import re
import tokenize
from collections import defaultdict
from pathlib import Path


def is_text_file(path: Path) -> bool:
    """Check whether a file contains text data."""
    try:
        with path.open("rb") as f:
            sample = f.read(1024)
        sample.decode("utf-8")
        return True
    except UnicodeDecodeError:
        return False


def normalize_content(text: str, ext: str) -> str:
    """Normalize file content by removing comments and whitespace."""
    if ext == ".py":
        tokens = []
        for tok in tokenize.generate_tokens(io.StringIO(text).readline):
            if tok.type not in {
                tokenize.COMMENT,
                tokenize.NL,
                tokenize.NEWLINE,
                tokenize.INDENT,
                tokenize.DEDENT,
            }:
                tokens.append(tok.string)
        return "".join(tokens)
    if ext in {".c", ".h"}:
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        text = re.sub(r"//.*", "", text)
    else:
        text = re.sub(r"#.*", "", text)
    return "".join(text.split())


def compute_signature(path: Path) -> str | None:
    """Return a stable signature for the logical content of a file."""
    if not is_text_file(path):
        return None
    try:
        text = path.read_text(encoding="utf-8", errors="ignore")
    except OSError:
        return None
    normalized = normalize_content(text, path.suffix.lower())
    if not normalized:
        return None
    return hashlib.md5(normalized.encode("utf-8")).hexdigest()


def find_duplicates(root: Path) -> dict[str, list[Path]]:
    """Return mapping of content signatures to list of duplicate files."""
    signatures: defaultdict[str, list[Path]] = defaultdict(list)
    for p in root.rglob("*"):
        if p.is_file():
            sig = compute_signature(p)
            if sig:
                signatures[sig].append(p)
    return {s: ps for s, ps in signatures.items() if len(ps) > 1}


def main() -> None:
    """Entry point for command-line execution."""
    root = Path.cwd()
    duplicates = find_duplicates(root)
    for files in duplicates.values():
        print("Duplicate logic found in:")
        for f in files:
            print(f"  {f}")
        print()


if __name__ == "__main__":
    main()
