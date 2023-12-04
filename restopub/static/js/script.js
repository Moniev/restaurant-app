const navigateHome = () => {
    $(document).ready(function () {
        $(".home__button").click(function () {
            $.ajax({
                type: 'GET',
                url: '/home',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/home"
                }
            });
        })
    })
}

const navigateAbout = () => {
    $(document).ready(function () {
        $(".about__button").click(function () {
            $.ajax({
                type: 'GET',
                url: '/about',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/about"
                }
            });
        })
    })
}

const navigateMenu = () => {
    $(document).ready(function () {
        $(".menu__button").click(function () {
            $.ajax({
                type: 'GET',
                url: '/menu',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/menu"
                }
            });
        })
    })
}

const navigateReserve = () => {
    $(document).ready(function () {
        $(".reserve__button").click(function () {
            $.ajax({
                type: 'GET',
                url: '/reserve',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/reserve"
                }
            });
        })
    })
}
/*
const navigateLogIn = () => {
    $(document).ready(function () {
        $("#").click(function () {
            $.ajax({
                type: 'GET',
                url: '/about',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/login"
                }
            });
        })
    })
}

const navigateSignUp = () => {
    $(document).ready(function () {
        $("#").click(function () {
            $.ajax({
                type: 'GET',
                url: '/about',
                beforeSend: function (request) {
                    request.setRequestHeader("resto_jwt", localStorage.getItem("resto_jwt"));
                },
                contentType: "application/json",
                success: function (response) {
                    window.location.href = "/register"
                }
            });
        })
    })
}
*/

const signUpAction = () => {
    $(document).ready(function () {
        $(".sign_up__button").click(function () {
            var name = document.getElementById("register__name").value;
            var lastname = document.getElementById("register__last_name").value;
            var email = document.getElementById("register__email").value;
            var password = document.getElementById("register__password").value;
            $.ajax({
                type: "POST",
                url: "/register_user/" + name + "/" + lastname + "/" + email + "/" + password + "/",
                contentType: "application/json",
                success: function (response) {
                    localStorage.setItem("resto_jwt", response)
                    window.location.href = "/register"
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
            console.log(localStorage.getItem("resto_jwt"));
            var email = document.getElementById("login__email").value;
            var password = document.getElementById("login__password").value;
            $.ajax({
                type: "GET",
                url: "/validate_login/" + email + "/" + password + "/",
                contentType: "application/json",
                success: function (response) {
                    localStorage.setItem("resto_jwt", response)
                    console.log(localStorage.getItem("resto_jwt"));
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
            localStorage.removeItem("resto_jwt");
        })
    })
}

const copyToClipBoard = () => {
    $(document).ready(function () {
        $(".phone").click(function () {
            document.execCommand("copy");
        })
    })
};


loginAction();
signUpAction();
navigateHome();
navigateAbout();
navigateMenu();
navigateReserve();