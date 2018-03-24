# SDQAnalyzer

This is a simple analyzer for the SDQ protocol that is used on a lot of Apple products, for exapmle:

- Lightning
- MagSafe
- iPhone/iPad battery

![Example of the analyzer in action](screenshots/example.png)

It is based on the Saleae example analyzer.

## Building

First, make sure that you cloned the repository including the submodules:

```
git clone --recursive https://github.com/nezza/SDQAnalyzer.git
```

And then simply run:

```
python3 build_analyzer.py
```
