function getCookie(_name) {
    const name = _name + "=";
  const cDecoded = decodeURIComponent(document.cookie);
  const cArr = cDecoded.split('; ');
  let res;
  cArr.forEach(val => {
    if (val.indexOf(name) === 0) res = val.substring(name.length);
  })
  return res
}


const navigateHome = () => {
    $(document).ready(function () {
        $(".home__button").click(function () {
            window.location.href = "/home";
        })
    })
}

const navigateAbout = () => {
    $(document).ready(function () {
        $(".about__button").click(function () {
            window.location.href = "/about";
        })
    })
}

const navigateMenu = () => {
    $(document).ready(function () {
        $(".menu__button").click(function () {
            window.location.href = "/menu";
        })
    })
}

const navigateReserve = () => {
    $(document).ready(function () {
        $(".reserve__button").click(function () {
            window.location.href = "/reserve";
        })
    })
}


/*
const navigateLogIn = () => {
    $(document).ready(function () {
        $("#").click(function () {
            window.location.href = "/login"
        })
    })
}

const navigateSignUp = () => {
    $(document).ready(function () {
        $("#").click(function () {
            window.location.href = "/register";
        })
    })
}
*/

const signUpAction = () => {
    $(document).ready(function () {
        $(".sign_up__button").click(function () {
            var nickname = document.getElementById("register__nickname").value;
            var name = document.getElementById("register__name").value;
            var lastname = document.getElementById("register__last_name").value;
            var email = document.getElementById("register__email").value;
            var password = document.getElementById("register__password").value;
            $.ajax({
                type: "POST",
                url: "/register_user/" + nickname + "/" + name + "/" + lastname + "/" + email + "/" + password + "/",
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/activation";
                },
                error: function () {
                    console.log("alfa")
                }
            })
        })
    })
}

const loginAction = () => {
    $(document).ready(function () {
        $(".log_in__button").click(function () {
            const email = document.getElementById("login__email").value;
            const password = document.getElementById("login__password").value;
            console.log("STEC")
            $.ajax({
                type: "POST",
                url: "/validate_login/" + email + "/" + password + "/",
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/home";
                },
                error: function () {
                    console.log("alfa")
                }
            })
        })
    })
}

const logoutAction = () => {
    $(document).ready(function () {
        $(".log_out__button").click(function () {
            const session_id = getCookie("resto_session");
            console.log(session_id)
            $.ajax({
                type: "POST",
                url: "/logout_user/",
                contentType: "application/json",
                success: function (response) {
                    if (response) {
                        window.location.href = "/home";
                    }
                },
                error: function () {
                    console.log("error")
                }
            })
        })
    })
};

const copyToClipBoard = () => {
    $(document).ready(function () {
        $(".phone").click(function () {
            document.execCommand("copy");
        })
    })
};


loginAction();
logoutAction();
signUpAction();
navigateHome();
navigateAbout();
navigateMenu();
navigateReserve();