#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <QString>
#include <QStringList>
#include "common.h"

class PostDetails
{
public:
    //TODO: support std::move
    PostDetails();
    explicit PostDetails(const QStringList& list);

    ~PostDetails();

    void set_subject(const QString& str);
    void set_date(const QString& str);
    void set_author(const QString& str);
    void set_size(const QString& str);
    void set_clicks(const QString& str);
    void set_replies(const QString& str);
    void set_body(const QString& str);

    const QString& get_subject() const
    {
        return m_subject;
    }

    const QString& get_date() const
    {
        return m_date;
    }

    const QString& get_author() const
    {
        return m_author;
    }

    const QString& get_size() const
    {
        return m_size;
    }

    const QString& get_clicks() const
    {
        return m_clicks;
    }

    const QString& get_replies() const
    {
        return m_replies;
    }

    const QString& get_body() const
    {
        return m_body;
    }

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
public:
    PageContent();
    ~PageContent();

    void add_post(const PostDetails& post);
    PostDetails get_post(int32_t index);
//    PostDetails& get_post(int32_t index);
    QStringList get_subject_list();
    std::vector<PostDetails> get_posts();


private:
    std::vector<PostDetails> m_posts;
};

#endif // PAGE_H
