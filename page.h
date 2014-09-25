#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <QString>
#include <QStringList>
#include "common.h"

class PostDetails
{
    //TODO: support std::move
    PostDetails();
    ~PostDetails();

    void set_subject(const QString& str);
    void set_date(const QString& str);
    void set_author(const QString& str);
    void set_size(const QString& str);
    void set_clicks(const QString& str);
    void set_replies(const QString& str);
    void set_body(const QString& str);

private:
    QString m_subject;
    QString m_date;
    QString m_author;
    QString m_size;
    QString m_clicks;
    QString m_replies;
    QString m_body;
};

class PageContent
{
    PageContent();
    ~PageContent();

    void add_post(const PostDetails& post);
    post get_post(int32_t index);
    QStringList get_subject_list();


private:
    std::vector<PostDetails> m_posts;
};

#endif // PAGE_H
