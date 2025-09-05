<!--
SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
SPDX-License-Identifier: MIT
-->

# Ruqola Setup on WSL/WINDOWS in Two Installs: Direct or with Docker

## Prerequisites

- **Windows**
- **Docker**
- **VcXsrv**
- **Git Bash**

---

## Direct Install

### **0. Install WSL openSUSE-Tumbleweed**

```powershell
wsl --install -d openSUSE-Tumbleweed
```

### **1. Install Docker and Git Bash**

- **Docker**: [Download here](https://docs.docker.com/desktop/setup/install/windows-install/)
- **Git Bash**: [Download here](https://git-scm.com/downloads/win)

**NOTES:**

- Enable Docker on WSL openSUSE-Tumbleweed using the widget provided.
- Test it using:

  ```sh
  docker -v
  ```

### **2. Install X Server**

- **VcXsrv**: [Download here](https://sourceforge.net/projects/vcxsrv/)

**Run the X Server:**

- Start **XLaunch**, select:
  - **Multiple Windows**
  - **Start No Client**
  - **Enable "Disable access control"**

### **3. Configure WSL**

Start WSL openSUSE-Tumbleweed:

```sh
chmod +x init.sh
./init.sh
```

**NOTES:**

- This configures WSL openSUSE.
- If you encounter repository errors, retry later.
- Ignore missing dependency warnings by pressing "c".
- If you face build errors in Ruqola, check `.bashrc`
  settings or consult online resources.

```sh
chmod +x additional.sh
./additional.sh
```

**NOTES:**

- Creates a virtual environment for `reuse lint`.
- Test it with:

  ```sh
  activate_lint_env
  reuse --version
  deactivate
  ```

#### **Test X Server for GUI Forwarding**

```sh
echo 'export DISPLAY=$(ip route | grep default | awk "{print \$3}"):0' >> ~/.bashrc
source ~/.bashrc
xclock
```

**NOTES:**

- Use either X Server or WSL2g. X Server is recommended for smoother performance.

#### **Run Ruqola Client**

```sh
cd ruqola
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=install ..
make
./bin/ruqola
```

To install Ruqola Client(optional):

```sh
make install
```

#### **Run and Install Rocket.Chat Server**

```sh
cd docker
```

- **Deploy with Docker & Docker Compose**: [Tutorial](https://docs.rocket.chat/v1/docs/deploy-with-docker-docker-compose)

If `docker-compose up -d` fails, inspect logs:

```sh
docker logs <container>
```

**NOTES:**

- Set PORT to `3000`.
- Store Docker-related files in `ruqola/docker/`.
  Ensure `.gitignore` includes `compose.yml` and `.env`.

#### **Install Your Preferred IDE/Editor**

**NOTES:**

- If using VS Code, install the C/C++ IntelliSense extension.

**🚀 CONGRATULATIONS! YOU'VE COMPLETED THE DIRECT INSTALL!**

---

## Docker Install

### **0. Build the Docker Container**

#### **1. Clone Ruqola Repository with SSH**

```sh
git clone git@github.com:your-username/ruqola.git
cd ruqola
```

#### **2. Build Docker Image**

```sh
cd docker
docker build -t ruqola .
```

**NOTES:**

- You can clone the repository using Git Bash, WSL-Ubuntu, or WSL-openSUSE-Tumbleweed.

### **4. Run Docker**

#### **0. Start a New Container and Open a Shell**

Mount your local folder to the container:

```sh
docker run -it --name ruqola-container -v
       /your/local/path:/path/within/container ruqola /bin/bash
```

Check OS inside the container:

```sh
cat /etc/os-release | grep "opensuse-tumbleweed" | awk -F=' {print $2}'
```

If correct, configure DISPLAY:

```sh
echo "export DISPLAY=host.docker.internal:0" >> ~/.bashrc
source ~/.bashrc
```

Test the installation:

```sh
xclock
```

If the clock window appears, the X server is set up correctly.

To manage containers:

```sh
docker stop ruqola-container
```

```sh
docker start -i ruqola-container
```

```sh
docker ps -a
```

#### **1. Run Ruqola from Inside the Container**

```sh
cd build
cmake -DCMAKE_INSTALL_PREFIX=install ..
make
./bin/ruqola
```

To install Ruqola Client(optional):

```sh
make install
```

#### **2. Development Workflow**

- Navigate to your project folder and edit as usual.
- To build and run:

  ```sh
  docker start -i ruqola-container
  ./bin/ruqola
  ```

**🚀 CONGRATULATIONS! YOU'VE COMPLETED THE DOCKER INSTALL!**
