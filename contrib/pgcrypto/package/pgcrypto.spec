Summary:        Cryptographic functions for Cloudberry Database 
License:        PostgreSQL License        
Name:           pgcrypto
Version:        %{pgcrypto_ver}
Release:        %{pgcrypto_rel}
Group:          Development/Tools
Prefix:         /temp
AutoReq:        no
AutoProv:       no
Provides:       pgcrypto = %{pgcrypto_ver} 

%description
The Pgcrypto package provides cryptographic functions for the Cloudberry Database.

%install
mkdir -p %{buildroot}/temp
make -C %{pgcrypto_dir} install prefix=%{buildroot}/temp

%files
/temp
