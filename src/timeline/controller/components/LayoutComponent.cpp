#include "LayoutComponent.h"
#include <QDebug>
#include "ComponentBean.h"

BaseLayoutComponent::BaseLayoutComponent(QObject *parent):BaseComponent(parent){

}


int BaseLayoutComponent::count(){
    if(m_componentBean == nullptr){
        return 0;
    }
    return ((GroupComponentBeans*)m_componentBean)->count();
}

void BaseLayoutComponent::onUnBind(){

}

QString BaseLayoutComponent::childQml(int index){
    if(m_componentBean == nullptr){
        return "";
    }
    return ((GroupComponentBeans*)m_componentBean)->get(index)->qml;
}

void BaseLayoutComponent::attachComponent(int index, QObject *component){
    if(m_componentBean == nullptr){
        qDebug()<<"attachComponent Failed m_componentBean isNULL";
        return ;
    }
    auto bean = (((GroupComponentBeans*)m_componentBean)->get(index));
    qDebug()<<"attachComponent :"<<component<<" Bean:"<<bean<<" params:"<<bean->paramObj << " m_componentBean:"<<m_componentBean;
    bean->m_translations = m_translations;
    bean->component = (BaseComponent*)component;
    bean->bindQml(curClip(),curEffect());
    bean->component->onComponentShow();
}

void BaseLayoutComponent::onComponentHide(){
    if(m_componentBean == nullptr){
        return;
    }
    auto groupBean = (GroupComponentBeans*)m_componentBean;
    for(int i=0;i<groupBean->count();++i){
        auto bean = groupBean->get(i);
        if(bean->component != nullptr){
            bean->component->onComponentHide();
        }
    }
}

void BaseLayoutComponent::onComponentShow(){
    if(m_componentBean == nullptr){
        return;
    }
    auto groupBean = (GroupComponentBeans*)m_componentBean;
    for(int i=0;i<groupBean->count();++i){
        auto bean = groupBean->get(i);
        if(bean->component != nullptr){
            bean->component->onComponentShow();
        }
    }
}


static QMap<QString,QString> m_compoentSelectedTitles;
void StackLayoutComponent::onBindQml(QJsonObject &data){
    m_cacheTitleKey = "stack_layout_title_"+curClip()->resourceBean().resourceTypeStr()+"_"+m_componentBean->title;
    auto selectTitle = m_compoentSelectedTitles.value(m_cacheTitleKey,"");
    for(int i=0;i<count();++i){
        if(subTitle(i) == selectTitle){
            m_curIndex = i;
            break;
        }
    }

}

void StackLayoutComponent::setCurIndex(int index){
    m_compoentSelectedTitles[m_cacheTitleKey] = subTitle(index);
    if(m_curIndex >= 0 && ((GroupComponentBeans*)m_componentBean)->get(m_curIndex)->component != nullptr){
        ((GroupComponentBeans*)m_componentBean)->get(m_curIndex)->component->onComponentHide();
    }
    m_curIndex = index;
    if(index >= 0 && ((GroupComponentBeans*)m_componentBean)->get(index)->component != nullptr){
        ((GroupComponentBeans*)m_componentBean)->get(index)->component->onComponentShow();
    }
}

int StackLayoutComponent::getCurIndex(){
    return m_curIndex;
}


QString StackLayoutComponent::subTitle(int index){
    return translate(((GroupComponentBeans*)m_componentBean)->get(index)->title);
}


int ColumnLayoutComponent::childItemHeight(int index){
    return 40;
}
