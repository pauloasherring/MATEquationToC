#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QRegularExpression>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonProcess_clicked()
{
    QString text = ui->textEditOrignalText->toPlainText();
    QString text2 = ui->textBrowserProcessedText->toPlainText();
    QString outLineText, inLineText;
    QStringList outText;
    QStringList inText;
    QTextStream textStream(&text);
    QString line;
    if (text.size()  == 0 && text2.size() == 0)
        return;

    if (text.size() > 2 && text2.size() > text.size())
    {
        ui->textEditOrignalText->clear();
        return;
    }

    if (text.size() < 2 && text2.size() > 10)
    {
        ui->textEditOrignalText->append(text2);
        ui->textBrowserProcessedText->clear();
        return;
    }



    while (textStream.atEnd() == 0)
    {
        line = textStream.readLine();
        QRegularExpression reNumbers("([^.]|^)\\b([0-9]+)\\b(?![.])");
        QRegularExpression reHatExpoent("([a-zA-Z]+[0-9]*)\\^(\\d+\\.\\d+|\\d+)");
        //"([a-zA-Z]+[0-9]*)\^(\\d+\\.\\d+|\\d+)";
        QRegularExpression reHatParenthesis("(\\)\\^)((\\d+\\.\\d+|\\d+))");
                                         //"([a-zA-Z]+[0-9]*)\\^([\\d.0]+)"
                                         //([a-zA-Z]+[0-9]*)\\^([\\d.0]+)

        outLineText = line;
        inLineText = line;
        outLineText.replace(reHatExpoent,"pow(\\1,\\2)");
        outLineText.replace(reNumbers,"\\1\\2.0");
        QRegularExpressionMatchIterator i;
        QStringList occorence;
        QStringList replacement;
        i = reHatParenthesis.globalMatch(outLineText);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            int endingIndex = match.capturedStart(0);
            QChar ccc = outLineText.at(endingIndex);
            int openParenthesisCount = 0;
            int runingIndex = endingIndex;
            endingIndex++;
            int parenthesisError = 0;
            int cont = 1;
            while (cont)
            {
                runingIndex--;
                ccc = outLineText.at(runingIndex);
                if (ccc == '(')
                {
                    if (openParenthesisCount == 0){
                        cont = 0;
                    }
                    else {
                        openParenthesisCount--;
                    }
                }
                else if ( ccc == ')')
                {
                    openParenthesisCount++;
                }
                if (runingIndex < 0)
                {
                    cont = 0;
                    parenthesisError = -1;
                }
            }
            if (!parenthesisError)
            {
                QString strReplace;
                strReplace = outLineText.mid(runingIndex,endingIndex-runingIndex); // this returns from ( to )
                QString strToMatch = outLineText.mid(runingIndex,endingIndex+match.captured(0).size()-1-runingIndex); // this returns the string from ( to )^N.nn
                int c = outLineText.count(strToMatch);
                if (c == 1)
                {
                    // one match only, we are good to go.
                    strReplace.prepend("pow");
                    strReplace.chop(1); // remove last )
                    strReplace.append(",");
                    strReplace.append(match.captured(2));
                    strReplace.append(")");
                    replacement.append(strReplace);
                    occorence.append(strToMatch);
                }
                else
                {
                    int a = 0;
                    a = 1;
                }
            }
        }
        for (int i = 0 ; i < replacement.size(); i++)
        {
            outLineText.replace(occorence.at(i),replacement.at(i));
        }

        inLineText.replace(reHatExpoent,"<font color=\"blue\">\\1^\\2</font>");
        inLineText.replace(reNumbers,"\\1<font color=\"red\">\\2</font>");
        inText.append(inLineText);
        outLineText.replace("^","<font color=\"red\">^</font>");
        outText.append(outLineText);
        //outText.append("\n");
        //inText.append("\n");
    }
    ui->textBrowserProcessedText->clear();
    for (int i = 0; i < outText.size(); i++)
    {
        ui->textBrowserProcessedText->append(outText.at(i));
    }
    //outText.replace("^","<font color=\"red\">^</font>");
    //ui->textBrowserProcessedText->append(outText);

    ui->textEditOrignalText->clear();
    for (int i = 0; i<inText.size(); i++)
    {
        ui->textEditOrignalText->append(inText.at(i));
    }
    return;

    // Fix ifs and whiles
    // list of words that uses end:
    //  if ... end      while ()        end         for ( )   end
    /*
     * sample:
     * if (vmax > sqrt(amax*r))
     *      vmax = sqrt(amax*r);
       end
       */
    text = ui->textEditOrignalText->toPlainText();
    QString strExp("(if)(.*end)");
    QRegularExpression reIf(strExp);
    reIf.setPatternOptions(QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator i;
    i = reIf.globalMatch(text);
    if (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString subStr = match.captured(2);

    }




}

