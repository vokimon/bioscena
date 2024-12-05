# Install instructions

```bash
sudo apt install scons libcppunit-dev
scons
cd config # or any dir containing Bioscena.ini, Agents.ini and OpCodes.ini
../bioscena
```

`bioscena` expects to find three config files in the working directory:

- `Bioscena.ini`: General parameters
- `Agents.ini`: Dinamic agents parameters
- `OpCodes.ini`: Byte codes to operation mapping



