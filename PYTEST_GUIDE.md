# pytest Integration Guide

## Quick Start

### Setup (one-time)
```bash
# Create and activate virtual environment
python3 -m venv .venv
source .venv/bin/activate

# Install pytest and plugins
pip install -r requirements-dev.txt
```

### Running Tests

```bash
# Activate venv
source .venv/bin/activate

# Run all Python tests
pytest tests/python/

# Run with verbose output (shows test names & status)
pytest tests/python/ -v

# Run specific test file
pytest tests/python/test_build_validation.py -v

# Run specific test class
pytest tests/python/test_build_validation.py::TestProjectStructure -v

# Run specific test
pytest tests/python/test_build_validation.py::TestProjectStructure::test_source_exists -v

# Generate beautiful HTML report
pytest tests/python/ -v --html=build_tests_pytest/report.html --self-contained-html

# Run only "smoke" tests (quick)
pytest tests/python/ -v -m smoke

# Run only "unit" tests
pytest tests/python/ -v -m unit

# Skip "slow" tests
pytest tests/python/ -v -m "not slow"

# Run tests in parallel (4 workers)
pytest tests/python/ -v -n 4

# Run with shorter output
pytest tests/python/ -q

# Stop on first failure
pytest tests/python/ -x

# Show local variables on failure
pytest tests/python/ -v -l
```

## File Structure

```
tests/
├── biz_logic/                    # Zephyr ztest (embedded C tests)
│   └── ...                       # Keep using west twister
└── python/                       # pytest (Python/integration tests)
    └── test_build_validation.py
conftest.py                       # Global pytest config & fixtures
pytest.ini                        # pytest settings
requirements-dev.txt              # Python dependencies
.venv/                            # Virtual environment (created)
```

## Markers (Tags)

```bash
# Mark tests with @pytest.mark.NAME:
@pytest.mark.smoke        # Quick sanity checks
@pytest.mark.unit         # Unit tests
@pytest.mark.integration  # Integration tests
@pytest.mark.slow         # Takes >1 second
```

## How pytest Fits In

| Test Type | Tool | What It Tests | Runs On |
|-----------|------|---------------|---------|
| **Embedded/Hardware** | ztest + west twister | GPIO, drivers, device logic | native_sim, qemu, hardware |
| **Integration/Build** | pytest | Build validation, config checks | Host machine |
| **CI/CD Scripts** | pytest | Automation, workflows | Host machine |

## No Interference!

✅ **pytest and ztest run independently**
- `west twister` still builds and runs Zephyr embedded tests normally
- `pytest` tests your build system, project structure, scripts
- Both can run in CI/CD without conflicts

## Zephyr Tests (Keep Using These!)

```bash
# Your existing ztest workflow is UNCHANGED:
west twister -vv -n -T tests/biz_logic --outdir build_tests_twister

# This still works exactly the same!
```

## Benefits of pytest Here

1. **Validate project structure** before building
2. **Test build scripts** and automation
3. **Check configuration files** for correctness
4. **Beautiful HTML reports** with colors and charts
5. **Parallel execution** for faster validation
6. **Better failure messages** than ztest
7. **Easy to write new integration tests**

## Example: Add Your Own Test

```python
# tests/python/test_my_feature.py
import pytest
from pathlib import Path

@pytest.mark.smoke
def test_my_feature(project_root):
    """Quick validation of my feature."""
    my_file = project_root / "src" / "myfile.c"
    assert my_file.exists()
    content = my_file.read_text()
    assert "my_feature" in content
```

Then run:
```bash
pytest tests/python/test_my_feature.py -v
```

## Fixtures Available

From `conftest.py`:

```python
def test_example(project_root, run_command):
    # project_root: Path object pointing to repo root
    
    # run_command: Execute shell commands
    result = run_command("west --version")
    assert result.returncode == 0
    print(result.stdout)
```

## Deactivate venv

```bash
deactivate
```

## Summary

- **Zephyr ztest tests**: `west twister -vv -T tests/biz_logic/` (unchanged)
- **Python tests**: `pytest tests/python/ -v` (new!)
- **HTML report**: `pytest tests/python/ -v --html=build_tests_pytest/report.html --self-contained-html`
- **Zero conflicts**: Run both systems independently
