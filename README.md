# c-pdb

An experimental PDB parser and dumper.

## Dumper Usage

```
dumper <pdb-file> <output-dir> [base-address] [--unroll-functions|-u]
```

| Name | Description |
|-:|-|
| `<pdb-file>` | The path of the PDB file to dump. |
| `<output-dir>` | The path of the folder to dump to. |
| `[base-address]` | The base address to add when resolving an RVA (optional). |
| `[--unroll-functions]` or `[-u]` | Unroll function block and variable scopes into their bodies (optional). |
