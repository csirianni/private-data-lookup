script_folder="/Users/stellaljung/Desktop/cs2390/private-data-lookup/backend/build"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
for v in PATH
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
    fi
done


export PATH="/Users/stellaljung/.conan2/p/cmakedf2c94b672dad/p/CMake.app/Contents/bin:$PATH"