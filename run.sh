

  git config --global user.email "sheng00125@qq.com"
  git config --global user.name "sheng"

git@github.com: Permission denied (publickey).

https://docs.github.com/cn/authentication/troubleshooting-ssh/error-permission-denied-publickey
https://github.com/settings/keys

ssh-keygen -t ed25519 -C "sheng00125@qq.com"


Generating public/private ed25519 key pair.

Enter file in which to save the key (/home/turtle/.ssh/id_ed25519): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/turtle/.ssh/id_ed25519.
Your public key has been saved in /home/turtle/.ssh/id_ed25519.pub.
The key fingerprint is:

SHA256:DVBva6CtEGi1WPSzVw00Q7e/WR+iabbh1E8UZuaaAFw 

sheng00125@qq.com

ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIPqIIdKhfkRzRSJajUURN3DbnMBi0WGLRFNYSNmbQLIs sheng00125@qq.com


The key's randomart image is:
+--[ED25519 256]--+
|   .+ ...o*E.    |
|   = o ....* .   |
|  + o o oo+ o  = |
| .   . = *.. .= .|
|    . o S +. ..+.|
|     . o .  = =+o|
|      .    B +o..|
|          = o o  |
|           o   . |
+----[SHA256]-----+