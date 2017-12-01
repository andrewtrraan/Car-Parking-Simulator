def send_email(to , subject):
    import smtplib
    import time

    FROM = 'garagesensor150@gmail.com'
    TO = to
    SUBJECT = ""
    if (subject == "enter"):
        SUBJECT = "Your Car Has Entered the Garage"
    elif (subject == "exit"):
        SUBJECT = "Your Car Has Exited the Garage"

    TEXT = "Car " + subject + "ed garage at " + time.strftime('%a %H:%M:%S')
    message = """From: %s\nTo: %s\nSubject: %s\n\n%s
          """ % (FROM, ", ".join(TO), SUBJECT, TEXT)
    username = 'garagesensor150@gmail.com'
    password = 'PaulWard'
    server = smtplib.SMTP('smtp.gmail.com:587')
    server.starttls()
    server.login(username, password)
    server.sendmail(FROM, TO, message)
    server.quit()

send_email('abdum.shk@gmail.com', "enter")