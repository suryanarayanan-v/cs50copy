import os
import re

from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    stock_rows = db.execute('SELECT * FROM stock_list WHERE user_id=:user_id', user_id=session['user_id'])
    user_rows = db.execute('SELECT * FROM users WHERE id=:id', id=session['user_id'])
    stock_name = []
    total_price = []
    user_value = user_rows[0]['cash']
    user_cash = user_value
    for row in stock_rows:
        stock = lookup(row['symbol'])
        stock_name.append(stock['name'])
        total_price.append(usd(row['cost'] * row['shares_owned']))
        user_value += (row['cost'] * row['shares_owned'])

    return render_template('index.html', rows=stock_rows, stock_name=stock_name, total_price=total_price,
                           user_value=usd(user_value), user_cash=usd(user_cash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template('buy.html')
    else:
        stock = lookup(request.form.get('symbol'))
        if not stock:
            return apology('INVALID STOCK')
        if not request.form.get('shares') or int(request.form.get('shares')) < 1:
            return apology('invalid no of shares')

        users_cash = db.execute('SELECT cash FROM users WHERE id=:user_id',
                                user_id=session['user_id'])

        total_cost = (stock['price'] * int(request.form.get('shares')))

        if users_cash[0]['cash'] < total_cost:
            return apology("can't afford shares")

        db.execute('INSERT INTO stock_list (user_id, shares_owned, cost, symbol) '
                   'values (:userid, :sharesowned, :cost, :symbol)', userid=session['user_id'],
                   sharesowned=int(request.form.get('shares')), cost=stock['price'],
                   symbol=request.form.get('symbol'))
        db.execute('UPDATE users SET cash=:cash WHERE id=:id', cash=users_cash[0]['cash'] - total_cost,
                   id=session['user_id'])

        now = datetime.now()
        formatted_date = now.strftime('%Y-%m-%d %H:%M:%S')

        db.execute('INSERT INTO history (user_id, symbol, shares, price, transacted) VALUES (:user_id, :symbol, '
                   ':shares, :price, :transacted)', user_id=session['user_id'], symbol=request.form.get('symbol'),
                   shares=int(request.form.get('shares')), price=stock['price'], transacted=formatted_date)

        return redirect('/')


@app.route("/history")
@login_required
def history():
    rows = db.execute('SELECT * FROM history WHERE user_id=:user_id', user_id=session['user_id'])
    return render_template('history.html', rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    get = True
    if request.method == 'GET':
        return render_template('quote.html', get=get)
    else:
        get = False
        stock = lookup(request.form.get('symbol'))
        if not stock:
            return apology('Please enter a valid symbol')
        return render_template('quote.html', get=get, name=stock['name'], price=usd(stock['price']),
                               symbol=stock['symbol'])


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template('register.html')
    else:
        if not request.form.get('username'):
            return apology("A username is required to create an account")
        if not request.form.get('password'):
            return apology("A password is required to create an account")
        if not request.form.get('confirmation'):
            return apology("Please enter password confirmation")
        if not request.form.get('password') == request.form.get('confirmation'):
            return apology('Passwords dont match')
        username = request.form.get('username')
        password = request.form.get('password')

        flag = 0
        while True:
            if len(password) < 8:
                flag = -1
                break
            elif not re.search("[a-z]", password):
                flag = -1
                break
            elif not re.search("[A-Z]", password):
                flag = -1
                break
            elif not re.search("[0-9]", password):
                flag = -1
                break
            else:
                flag = 0
                break

        if flag == -1:
            return apology('Please choose a stronger password')

        password_hash = generate_password_hash(password)
        db.execute('INSERT INTO users (username, hash) VALUES (:username, :password_hash)',
                   username=username, password_hash=password_hash)
        return redirect('/login')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        rows = db.execute('SELECT * FROM stock_list WHERE user_id=:user_id', user_id=session['user_id'])
        return render_template('sell.html', rows=rows)
    else:
        symbol = request.form.get('symbol')
        if not symbol:
            return apology('Please select a stock')
        rows = db.execute('SELECT * FROM stock_list WHERE user_id=:user_id AND symbol=:symbol',
                          user_id=session['user_id'], symbol=symbol)
        cash = db.execute('SELECT * FROM users WHERE id=:id', id=session['user_id'])

        shares = rows[0]['shares_owned']
        sell_shares = int(request.form.get('shares'))

        if shares < sell_shares:
            return apology('You dont have shares')
        if sell_shares < 1:
            return apology('You cannot sell shares lower than 1')
        stock = lookup(symbol)
        if shares == sell_shares:
            db.execute('DELETE FROM stock_list WHERE symbol=:symbol AND user_id=:user_id',
                       user_id=session['user_id'], symbol=symbol)
        else:
            db.execute('UPDATE stock_list SET shares_owned=:shares_owned WHERE user_id=:user_id AND symbol=:symbol',
                       shares_owned=shares - sell_shares, user_id=session['user_id'], symbol=symbol)
    db.execute('UPDATE users SET cash=:cash WHERE id=:id', cash=cash[0]['cash'] + (stock['price'] * sell_shares),
               id=session['user_id'])

    now = datetime.now()
    formatted_date = now.strftime('%Y-%m-%d %H:%M:%S')

    db.execute('INSERT INTO history (user_id, symbol, shares, price, transacted) VALUES (:user_id, :symbol, '
               ':shares, :price, :transacted)', user_id=session['user_id'], symbol=symbol,
               shares=(sell_shares*-1), price=stock['price'], transacted=formatted_date)

    return redirect('/')


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
