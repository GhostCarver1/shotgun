async function login(email, password)
{
    const response = await fetch("/login", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            email: email,
            password: password
        })
    });

    const data = await response.json();

    localStorage.setItem("json_output", JSON.stringify(data));
    
    alert(data.status);

    if (data.status==="success")
    {
        const user = {
            user_email: email,
            user_password: password,
            user_name: data.user_name,
            user_token: data.token,
            user_id: data.user_id
        };

        localStorage.setItem("user", JSON.stringify(user));
    }

}

async function permission(id, token) {
    const response = await fetch("/permission", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            id: id,
            token: token
        })
    });

    const data = await response.json();

    localStorage.setItem("json_output", JSON.stringify(data));

    document.getElementById("user_permission").innerHTML = data.permission;
    
}

async function signup(user_name, email, password)
{
    const response = await fetch("/signup", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            email: email,
            password: password,
            user_name: user_name
        })
    });

    const data = await response.json();

    localStorage.setItem("json_output", JSON.stringify(data));
    
    alert(data.status);

    if (data.status==="success")
    {
        const user = {
            user_email: email,
            user_password: password,
            user_name: data.user_name,
            user_token: data.token,
            user_id: data.user_id
        };

        localStorage.setItem("user", JSON.stringify(user));
    }

}