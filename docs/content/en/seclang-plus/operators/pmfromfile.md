+++
title = "@pmFromFile"
weight = 11
+++

**Description:** Load keywords from file for parallel matching

**Syntax:** `@pmFromFile /path/to/keywords.txt`

**Input Data Type:** `string (file path)`

When too many keywords are used in `@pm`, rules can become very long and difficult to maintain. In that case, `@pmFromFile` is preferred. It loads all patterns from a file. WGE must have read permission for that file.

`@pmFromFile` and `@pm` are both implemented with Hyperscan. To enhance `@pmFromFile`, WGE supports extra control directives in the pattern file:

**Options:**
- `##!+ i`: case-insensitive matching
- `##!+ -i`: case-sensitive matching
- `##!+ l`: literal string matching
- `##!+ -l`: regex matching
- `##!^ prefix_string`: prepend a prefix to all following patterns
- `##!$ suffix_string`: append a suffix to all following patterns
- `##`: stop parsing early; following patterns are ignored
- `#`: comment line (ensure it does not match control directive formats above)

Among these options, `##!+ l` and `##!+ -l` are global exclusive switches. For one `@pmFromFile` rule, matching mode is either regex or literal, determined by the last one seen.

Other options can be applied partially by scope. For example, a subset can be case-insensitive and another subset case-sensitive:

```
# Patterns below are case-insensitive
##!+ i
admin
user
passwd
# Patterns below are case-sensitive
##!+ -i
root
bash
echo
```

Prefix/suffix options can also be applied partially, and you can reset them directly with `##!^ ` and `##!$ `.

**Default:** case-insensitive, no prefix/suffix, literal matching.

**Example:**

```apache
# Load malicious User-Agent list from file
SecRule REQUEST_HEADERS:User-Agent "@pmFromFile /etc/wge/bad-ua.txt" \
    "id:1001,phase:1,deny,msg:'Malicious User-Agent'"
```
