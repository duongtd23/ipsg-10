## IPSG

Formal verification TLS 1.2 with IPSG.

The tool requires Maude alpha tool, which can be found here https://github.com/SRI-CSL/Maude.

### How to run:

```
    $ maude -allow-files ipsg.maude
```
After the tool started, for example, we can use the following command:
```
    $ load tls12/input2+3.cafe .
```