# 🐚 minishell

Welcome to **minishell**, a small but powerful UNIX shell built in C as part of the [42/1337](https://1337.ma) school curriculum. It mimics the behavior of Bash by supporting environment variables, redirections, pipelines, signal handling, and a full set of built-in commands — all developed from scratch using only allowed C library functions.

---

## 🚀 Features

- ✅ Custom **prompt** that appears when waiting for input
- ✅ **Command history** using `readline`
- ✅ **PATH resolution** to locate and execute binaries
- ✅ **Builtins** implemented internally:
  - `echo` with `-n`
  - `cd` (relative/absolute paths)
  - `pwd`
  - `export`, `unset`
  - `env`
  - `exit`
- ✅ **Environment variable expansion**
  - Supports `$VAR`, `$?`, and more
- ✅ **Pipelines** with `|`
- ✅ **Redirections**:
  - `>` — output redirection (overwrite)
  - `>>` — output redirection (append)
  - `<` — input redirection
  - `<<` — heredoc with a custom delimiter
- ✅ **Quoting rules**
  - Single quotes `'` prevent all expansions
  - Double quotes `"` preserve spaces but expand `$`
- ✅ **Signal handling**:
  - `Ctrl-C` interrupts and redraws prompt
  - `Ctrl-D` exits the shell
  - `Ctrl-\` is ignored
- ✅ Uses only **one global variable** to store signal state

---

## 🧪 Run & Compile

```bash
git clone https://github.com/yourusername/minishell
cd minishell
make
./minishell
```

## 🔍 What Makes This Shell Unique

Implementing a shell from scratch means rebuilding a significant part of what makes Bash work — and it’s not just about calling `execve`. I had to reimplement tokenization, environment variable expansion, signal-safe execution, redirection mechanics, and internal command routing.

### 🧩 Parsing & Tokenization

Shell parsing isn't just about splitting on spaces. Tokens can include:
- **Quoted substrings**
- **Variables like `$USER`**
- **Redirection symbols** (`<`, `>`, `>>`, `<<`)
- **Edge cases like spaces inside quotes** or combinations like:


At first, parsing and tokenizing was one of the hardest parts of the project. I had to:
- Preserve quoted segments as a single token
- Prevent splitting variable-expanded results unless unquoted
- Track open/closed quote states to catch syntax errors

### 🏗️ Redirections & Pipelines

Redirections and pipes required low-level file descriptor manipulation:
- `dup2()` was used to reroute input/output streams
- File descriptors were opened/closed correctly to avoid leaks
- Multiple chained redirections and pipes were handled cleanly

For example:
- Input redirection from a file with `<`
- Output appending with `>>`
- Chaining commands using a pipe between them

### ⚙️ Builtins

Some builtins like `echo` were simple, but others like `export`, `cd`, and `exit` were more subtle:
- `cd` had to update both `PWD` and `OLDPWD`
- `export` needed parsing of `KEY=VALUE` format and validation
- `exit` had to validate numeric arguments and return correct exit codes

---

## ⚠️ Not Implemented

To stay within project scope and subject requirements, this shell:
- ❌ Does not handle unclosed quotes (they produce syntax errors)
- ❌ Does not support escape sequences like `\`
- ❌ Does not support command substitution (e.g., `$(cmd)`)

---

## 🧠 What I Learned

- How shells work at a deep level: process creation, piping, redirection, signal masking
- Writing a reliable tokenizer and parser that handles real-world Bash syntax
- The importance of modular and clean code structure
- How to manage subprocesses, avoid zombies, and restore terminal state

---

## 🔧 Tech Stack

- Language: **C**
- Libraries: [`readline`](https://tiswww.case.edu/php/chet/readline/rltop.html), POSIX standard
- Tools: `valgrind` (for testing and debugging)

---

## 📂 Example Usage

You can run commands like:
```bash
# Builtins
echo Hello, world!
echo -n "No newline"
cd ..
pwd
export NAME=MiniShell
env
unset NAME
exit

# Environment Variables
echo $USER
echo "Hello, $USER!"
echo "$USER is logged in"
echo $HOME/Desktop
echo $?

# Quotes Handling
echo "This is a quoted string"
echo 'Single quotes: $USER is not expanded'
echo "Double quotes: $USER is expanded"
echo "Mixing"quotes" and "$USER

# Redirections
echo Hello > file.txt
cat < file.txt
echo Another line >> file.txt
cat < input.txt > output.txt

# Pipes
ls -l | grep minishell
cat file.txt | wc -l
ps aux | grep bash | sort -k 2 -n

# Heredoc
cat << EOF
This is a heredoc test.
Multiple lines until EOF.
EOF

# Combined Redirections & Pipes
cat < input.txt | grep keyword > result.txt
grep "error" logfile.txt | wc -l >> errors_count.txt

# Exit Status
false
echo $?   # prints 1
true
echo $?   # prints 0
```
## 🏁 Final Thoughts

Building this shell gave me a deeper appreciation for how much logic is hidden behind the command line we all take for granted. What started off as a simple prompt turned into a full-featured interpreter with proper process and memory management.

---

## 📸 Screenshots 

![Screenshot from 2025-06-03 14-24-33](https://github.com/user-attachments/assets/12a21a67-f0e1-498f-ae28-b029e7f83d0d)

---
